# ESP8266 / ESP32 + DWM1000 UWB Ranging

Two-way ranging (TWR) between ESP8266 (NodeMCU) and ESP32 boards equipped with
Decawave DWM1000 UWB modules. One **tag** (initiator) measures distance to one or
more **anchors** (responders) and reports over UDP.

## Hardware / pinout

### ESP8266 (NodeMCU)

| Signal  | GPIO   | NodeMCU label |
|---------|--------|---------------|
| RST     | GPIO5  | D1            |
| SS (CS) | GPIO15 | D8            |
| IRQ     | GPIO4  | D2            |
| MOSI    | GPIO13 | D7            |
| MISO    | GPIO12 | D6            |
| SCK     | GPIO14 | D5            |

> ⚠️ Do **not** put RST on GPIO1 (TX). GPIO1 is the UART transmit pin, so the
> DWM1000 reset pulse breaks the serial console. Note that `D1` = GPIO5, it is
> **not** GPIO1.

### ESP32

See `esp32_pinout_uext.md` for the wiring used by `tag_esp32/`:

| Signal  | GPIO |
|---------|------|
| RST     | 19   |
| SS (CS) | 32   |
| IRQ     | 36   |
| MOSI    | 23   |
| MISO    | 34   |
| SCK     | 17   |

> `MISO` (34) and `IRQ` (36) are input-only pins on the ESP32 — correct for their
> roles, but they cannot be used as outputs.

## Layout

Six independent PlatformIO projects, one per board:

- `tag/`       — ESP8266 initiator, reports UWB tag events over UDP
- `tag_esp32/` — ESP32 initiator, reports UWB tag events over UDP
- `anchor/`    — responder 1, UWB address `82:17:5B:D5:A9:9A:E2:9C`
- `anchor2/`   — responder 2, UWB address `82:17:5B:D5:A9:9A:E2:9D`
- `anchor3/`   — responder 3, UWB address `82:17:5B:D5:A9:9A:E2:9E`
- `anchor4/`   — responder 4, UWB address `82:17:5B:D5:A9:9A:E2:9F`
- `gui/`       — Rust UDP server + egui dashboard (the central server)

Each project has its own `platformio.ini`, `src/` sketch and a vendored
`lib/DW1000`.

## DWM1000 library — IMPORTANT

The project uses [thotro/arduino-dw1000](https://github.com/thotro/arduino-dw1000),
but it is **vendored** (copied into `lib/DW1000` in each project) rather than
pulled from `lib_deps`, because the stock library needs three patches to run on
the ESP8266. The patched copy is already in place — you do **not** need to
re-apply the patches to rebuild. They are documented here so the library can be
reproduced from a clean clone.

### Patch 1 — put the ISR in IRAM (`DW1000.h`)

The library registers `handleInterrupt()` with `attachInterrupt()`. On the
ESP8266 an interrupt service routine must reside in IRAM, otherwise the core
aborts with `ISR not in IRAM!`.

```diff
 	/* Arduino interrupt handler */
-	static void handleInterrupt();
+	static void IRAM_ATTR handleInterrupt();
```

### Patch 2 — same for the definition (`DW1000.cpp`)

```diff
-void DW1000Class::handleInterrupt() {
+void IRAM_ATTR DW1000Class::handleInterrupt() {
```

### Patch 3 — reset before attaching the IRQ (`DW1000.cpp`, `begin()`)

The original `begin()` attaches the interrupt *before* the chip is reset, so the
IRQ line is still asserted from a previous run. The ISR fires immediately and its
SPI reads from interrupt context hang the ESP8266. Hard-reset the chip first:

```diff
 	_deviceMode = IDLE_MODE;
+
+	// Hard-reset the chip before attaching the interrupt so its IRQ line is de-asserted.
+	if(_rst != 0xff) {
+		pinMode(_rst, OUTPUT);
+		digitalWrite(_rst, LOW);
+		delay(2);
+		pinMode(_rst, INPUT);
+		delay(10);
+	}
 	// attach interrupt
```

### Patch 4 — random backoff on the blink reply (`DW1000Ranging.cpp`)

All anchors answer the tag's discovery blink immediately, so their replies
collide in the air and some anchors never get discovered. Add a random delay
before replying:

```diff
 			if(addNetworkDevices(&myTag)) {
 				if(_handleBlinkDevice != 0) {
 					(*_handleBlinkDevice)(&myTag);
 				}
+				delay(random(0, 50));  // random backoff so anchors don't collide on the blink reply
 				//we reply by the transmit ranging init message
 				transmitRangingInit(&myTag);
 				noteActivity();
 			}
```

### Patch 5 — fixed short address (`DW1000Ranging.cpp`)

The library assigns a *random* 16-bit short address (seeded from `analogRead(0)`),
which can collide across boards. Use the last two bytes of the EUI instead, so
each device gets a deterministic unique short address:

```diff
 	else {
-		// we use first two bytes in addess for short address
-		_currentShortAddress[0] = _currentAddress[0];
-		_currentShortAddress[1] = _currentAddress[1];
+		// use the LAST two bytes: unique fixed short address per device (no collisions)
+		_currentShortAddress[0] = _currentAddress[6];
+		_currentShortAddress[1] = _currentAddress[7];
 	}
```

And pass `false` as the third argument so the sketches use the fixed address:

```cpp
DW1000Ranging.startAsAnchor(ANCHOR_ADDR, DW1000.MODE_LONGDATA_RANGE_ACCURACY, false);
DW1000Ranging.startAsTag(TAG_ADDR, DW1000.MODE_LONGDATA_RANGE_ACCURACY, false);
```

### Patch 6 — ESP32 SPI pins and interrupt (`DW1000.cpp`)

`tag_esp32/` needs two extra ESP32-specific changes on top of Patches 1-5:

1. Custom SPI pins — the ESP32 uses SCK=17, MISO=34, MOSI=23, not the default
   VSPI pins:

```diff
-	SPI.begin();
+	#ifdef ESP32
+		SPI.begin(17, 34, 23);  // SCK, MISO, MOSI (custom ESP32 pins)
+	#else
+		SPI.begin();
+	#endif
```

2. `SPI.usingInterrupt()` does not exist on ESP32 — delete that call.

3. `tag_esp32/` also releases `RSTn` with `INPUT_PULLUP` instead of floating it
   (redundant with the module pull-up, but more robust).

4. Mark the **whole IRQ call-chain `IRAM_ATTR`** — otherwise the ESP32 panics
   with `Guru Meditation Error: Interrupt wdt timeout` when the IRAM ISR calls
   flash-resident code. In `DW1000.cpp`: `handleInterrupt` (already done by
   Patch 1), `readSystemEventStatusRegister`, `readBytes`, `writeBytes`,
   `clearTransmitStatus`, `clearReceiveStatus`, `clearReceiveTimestampAvailableStatus`,
   `clearAllStatus`, `newReceive`, `startReceive`, and the `is*` status helpers.
   In `DW1000Ranging.cpp`: `handleSent`, `handleReceived`, `noteActivity`.

5. Add to `tag_esp32/platformio.ini`:

```ini
build_flags = -DCONFIG_DISABLE_HAL_LOCKS
```

   The ESP32 Arduino SPI takes a blocking mutex in `beginTransaction()`, and the
   DW1000 ISR also does SPI — when they overlap the ISR blocks forever and the
   watchdog fires. Disabling the HAL locks makes the ESP32 SPI lock-free (like
   the ESP8266), which fixes the deadlock.

6. Use a **slower SPI clock (8 MHz)** for the ESP32 — its DWM1000 is wired
   through a flat cable, so the default 16 MHz reads were marginal (device ID
   read `DFCA`/`FFFF` instead of `DECA`). In `DW1000.cpp` set the non-ESP8266
   `_fastSPI` to `SPISettings(8000000L, MSBFIRST, SPI_MODE0)`. This gives a
   clean `DECA` device-ID read.

### Build flag (`platformio.ini`)

The ESP8266 Arduino core compiles with `-Werror=return-type`, but the library's
`DW1000Ranging.cpp::detectMessageType()` falls off the end of a non-void function
without a return. Downgrade that one warning (present in every `platformio.ini`):

```ini
build_unflags = -Werror=return-type
```

### Reproducing the vendored library from a clean clone

```bash
git clone --depth 1 https://github.com/thotro/arduino-dw1000.git DW1000
# apply Patches 1-5 (and Patch 6 for the ESP32 project)
rm -rf DW1000/.git
cp -r DW1000 tag/lib/
cp -r DW1000 anchor/lib/
cp -r DW1000 anchor2/lib/
cp -r DW1000 anchor3/lib/
cp -r DW1000 anchor4/lib/
cp -r DW1000 tag_esp32/lib/
```

## Antenna delay calibration

All sketches call `DW1000.setAntennaDelay(16422)`. This value is
hardware-specific and was calibrated for these boards: the stock EVK1000 default
(16436) read about 13 cm too short at a true 10 cm separation.

To re-calibrate: place a tag and anchor exactly 1 m apart, read the reported
distance, and adjust the value (empirically ≈9 mm per unit) until it reads
1.00 m. Apply the same value to every sketch and reflash.

## Build & flash

```bash
cd tag && pio run -t upload --upload-port /dev/ttyUSB0
cd tag_esp32 && pio run -t upload --upload-port /dev/ttyUSB0
cd anchor && pio run -t upload --upload-port /dev/ttyUSB1
cd anchor2 && pio run -t upload --upload-port /dev/ttyUSB2
cd anchor3 && pio run -t upload --upload-port /dev/ttyUSB3
cd anchor4 && pio run -t upload --upload-port /dev/ttyUSB4
```

PlatformIO Core 6.x is used — the system `pio` 4.x from apt is broken under
Python 3.12. Use `~/.platformio/penv/bin/pio` (add it to `PATH`).

## OTA updates

Every device runs ArduinoOTA with an mDNS hostname:

| Hostname                 | Device   |
|--------------------------|----------|
| `uwb-tag1`               | ESP8266 tag |
| `uwb-tag2`               | ESP32 tag   |
| `uwb-anchor1` .. `uwb-anchor4` | anchors |

An optional OTA password comes from the `OTA_PASS` build-time env var (same
mechanism as `WIFI_SSID`/`WIFI_PASS`); when set, the device requires it for OTA.

After the first (USB) flash, update over the network, e.g.:

```bash
pio run -t upload --upload-port uwb-tag2.local
```

## UDP protocol

Devices speak a protobuf-over-UDP protocol (see `msgs.proto` and `protocol.md`).
Every message is an `Envelope { src, dst, msg_type, payload }` where `src` is the
FNV-1a 32-bit hash of the device name and `msg_type` selects the payload layout:

- `1 ANNOUNCE` — every 5 s, multicast to `239.0.0.23:50000` *and* unicast to the
  server; carries device name, capabilities, IP and UDP port.
- `2 SET_BROKER` — an incoming message that switches a device's server address.
- `3 SYS_EVENT` — every 30 s: uptime, WiFi MAC/IP and RSSI.
- `4 UWB_ANCHOR_EVENT` — on boot and every 30 s: the anchor's 8-byte EUI.
- `5 UWB_TAG_EVENT` — 4 Hz per anchor: the tag's EUI, the anchor short address,
  distance and RX power.

The firmware encoder/decoder lives in the shared `uwb_proto/uwb_proto.h`
(copied into each project's `src/`), and the matching decoder lives in
`gui/src/main.rs`. Field numbers were re-based to 1 (protobuf forbids 0) and
`wifi_rssi` is `int32` so negative dBm round-trips.

## Dashboard

The `gui/` crate is the central UDP server. Run it with:

```bash
cd gui && cargo run
```

It binds `0.0.0.0:50000`, decodes every envelope, and renders announced devices
plus a live distance / RX-power table per tag.

## Credentials

WiFi credentials are **not stored in the source**. They are injected at build
time from the environment variables `WIFI_SSID` and `WIFI_PASS` (via
`${sysenv.*}` in each `platformio.ini`). Build/flash with:

```bash
WIFI_SSID="MyNetwork" WIFI_PASS="secret" \
  pio run -t upload --upload-port /dev/ttyUSB0
```

The device name (and therefore the `src` hash) is set at the top of each
sketch (`tag/`, `tag_esp32/`, and `anchor/`..`anchor4/`). All of them use the
`WIFI_SSID`/`WIFI_PASS` build flags (and optionally `OTA_PASS` for OTA).
