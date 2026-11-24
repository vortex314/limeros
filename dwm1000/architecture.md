# DWM1000 UWB Ranging — Architecture

This document describes the *dwm1000* sub-project: a UWB indoor ranging system
built from ESP8266/ESP32 boards carrying Decawave DWM1000 modules. It sits inside
the larger `limeros/` repository and reuses its codegen toolchain
(`coder/`, `libs/protocol/`, `libs/uwb_proto/`). The sibling
[`../architecture.md`](../architecture.md) covers the Limeros middleware itself;
this file covers the UWB application only.

## 1. Overview

Two-way ranging (TWR) over DWM1000 ultra-wideband radios:

- One **tag** (initiator) measures the distance to one or more **anchors**
  (responders) using the three-frame TWR exchange.
- Tags and anchors all report to a **central server** (`gui/`) over WiFi/UDP.
- The central server renders a live distance / RX-power table per tag.

Devices are split across three PlatformIO projects (one firmware tree each) and
one Rust crate:

| Component   | Hardware | Role |
|-------------|----------|------|
| `tag/`       | ESP8266 (NodeMCU) | Initiator, tag `uwb_tag_1` |
| `tag_esp32/` | ESP32 | Initiator, tag `uwb_tag_2` |
| `anchor/`    | ESP8266 (NodeMCU) | Responders `uwb_anchor_1` … `uwb_anchor_5` (one env per anchor) |
| `gui/`       | Linux host | UDP server + egui dashboard |

## 2. System topology

```
                 UWB (TWR)                      WiFi / UDP (CBOR)
 ┌──────────┐                    ┌──────────┐
 │ anchor1  │◄──────────────────►│          │
 │ anchor2  │◄──────────────────►│   tag    │─── unicast/multicast ───► ┌──────────┐
 │ anchor3  │◄──────────────────►│ (8266/32)│                            │   gui    │
 │   ...    │                    │          │                            │ (server) │
 └──────────┘                    └──────────┘                            └──────────┘
```

- Anchors and the tag range over UWB only — the tag is the sole initiator.
- Every device independently reports over UDP to the server at
  `192.168.0.148:50000` (override with `-DUWB_SERVER_IP=…`).
- The server is passive: it never instructs a device to range. The only
  device-directed message is `DeviceRequest` (SetBroker), which repoints a
  device's server address.

## 3. Repository layout

```
dwm1000/
├── tag/                     ESP8266 tag (initiator)
│   ├── platformio.ini
│   └── src/esp8266_dwm1000_tag.ino
├── tag_esp32/               ESP32 tag (initiator)
│   ├── platformio.ini
│   ├── src/idf_component.yml
│   └── src/esp32_dwm1000_tag.ino
├── anchor/                  ESP8266 anchors (one env per anchor)
│   ├── platformio.ini       envs anchor1..anchor5
│   └── src/esp8266_dwm1000_anchor.ino
├── gui/                     Rust UDP server + egui dashboard
│   ├── Cargo.toml
│   └── src/main.rs
├── README.md                hardware, patches, build/flash, protocol summary
├── protocol.md              UDP/CBOR protocol notes (partly stale — see §6)
├── esp32_pinout_uext.md     ESP32 wiring
└── architecture.md          this file
```

Shared code lives one level up, in `limeros/`:

```
limeros/
├── libs/
│   ├── DW1000/              patched arduino-dw1000 (thotro) — the UWB driver
│   ├── uwb_proto/src/uwb_proto.h   UwbProto transport helper
│   ├── protocol/src/msgs.{h,cpp}   generated CBOR message classes (C++)
│   └── common/              shared helpers (fnv.h, Buffer/Option, msg.h)
└── coder/
    ├── hcl/robot.hcl        IDL (source of truth for message layout)
    └── generated/           generated Rust crate (used by gui/)
```

`anchor/` and `tag_esp32/` find the shared libs through
`lib_extra_dirs = ../../libs`; `tag/` (ESP8266) still pulls
`vortex314/arduino-dw1000#all-fixes` from `lib_deps`, so it does **not** get the
multi-anchor fixes (Patch 7). Prefer `anchor/` + `tag_esp32/` as the current
deployment.

## 4. Hardware / pinout

### ESP8266 (NodeMCU) — `tag/` and `anchor/`

| Signal  | GPIO   | NodeMCU label |
|---------|--------|---------------|
| RST     | GPIO5  | D1            |
| SS (CS) | GPIO15 | D8            |
| IRQ     | GPIO4  | D2            |
| MOSI    | GPIO13 | D7            |
| MISO    | GPIO12 | D6            |
| SCK     | GPIO14 | D5            |

> RST must **not** be GPIO1 (TX) — the reset pulse would break the serial console.

### ESP32 — `tag_esp32/`

| Signal  | GPIO |
|---------|------|
| RST     | 19   |
| SS (CS) | 32   |
| IRQ     | 36   |
| MOSI    | 23   |
| MISO    | 34   |
| SCK     | 17   |

`MISO` (34) and `IRQ` (36) are input-only pins — correct for their roles. The
ESP32 SPI pins are passed to the library via `-DDW1000_SPI_SCK/MISO/MOSI`
build flags, and the clock is derated to 8 MHz because the module is wired
through a flat cable.

## 5. Ranging model

### 5.1 Three-frame TWR exchange

The library implements the standard Decawave TWR scheme: the tag sends a **POLL**,
the anchor replies **POLL_ACK**, the tag sends **RANGE**, the anchor replies
**RANGE_REPORT**, and the tag computes distance from the four timestamps.

### 5.2 Multi-anchor scheduling

With a single tag and N anchors, the naive protocol collides: every anchor
answers the same POLL at once. The patched library (Patch 4 + Patch 7) slots the
replies:

- Discovery blinks are answered in one of `BLINK_REPLY_SLOTS` (16) random 4 ms
  slots, starting 2 ms after the blink.
- POLL_ACK for anchor *i* is delayed by `(i+1) * DEFAULT_REPLY_DELAY_TIME`
  (15, 30, 45 … ms), held as a `uint32_t` and sent in 10 µs units on the wire.
- The tag sends RANGE once the last reply slot (+10 ms) has elapsed, to the
  anchors that actually answered — a lost ack no longer cancels the whole round.
- Any frame from a known anchor counts as activity; `INACTIVITY_TIME` is 3000 ms.

### 5.3 Addresses

- The 8-byte EUI-64 is fixed per device in the sketch (see §7).
- The 16-bit **short address** is derived from the *last two* EUI bytes,
  little-endian (`…:E2:9C` → `0x9CE2`). This is deterministic and unique per
  board, replacing the library's random short address (Patch 5).

### 5.4 Antenna delay

`setAntennaDelay()` compensates per-board hardware latency. Values are calibrated
per device:

- `tag/` (ESP8266): `16422`
- `tag_esp32/` (ESP32): `16456`
- `anchor/`: per-anchor `ANTENNA_DELAY` from `platformio.ini`
  (`anchor1=16488`, `anchor2=16454`, `anchor3=16475`, `anchor4=16465`,
  `anchor5=16531`; default `16456`).

The anchor calibration was done by unicast ranging at 1.75 m (9-minute median)
against the ESP32 tag at `16456`; +1 unit shortens that anchor's reading by
~4.7 mm.

### 5.5 Measurement filtering (tag firmware)

`newRange()` applies two guards before a reading is accepted and reported:

- **Range gate** — reject `distance < -1 m` or `> 20 m` (timestamp/multipath
  glitches).
- **Jump filter** — reject a reading that deviates more than 2 m from the
  anchor's last accepted value (keeps the previous value).

`tag_esp32/` additionally marks an anchor stale after 10 s without a fresh range
and stops reporting it.

## 6. Networking & message protocol

### 6.1 Transport

- Each device opens a UDP socket on a **random port** (20000–60000).
- **Announce** (`DeviceEvent`) is sent every 5 s, multicast to
  `239.0.0.23:50000` *and* unicast to the server, so the server sees it even
  where multicast is not routed. (ESP32 skips the multicast send and relies on
  the unicast path.)
- **Events** (`SysEvent`, `DW1000AnchorEvent`, `DW1000TagEvent`) are unicast to
  the server.
- A device listens for incoming UDP; a `DeviceRequest` carrying
  `ip_address` + `port` repoints its server (SetBroker).

### 6.2 Envelope

Every datagram is a CBOR-encoded `Envelope`, serialized as a positional CBOR
array (fields in declaration order; absent fields are `null`):

```
Envelope {
  src:         uint32   // FNV-1a 32-bit hash of the device name
  dst:         uint32   // absent for events, filled for request/reply
  msg_type:    uint32   // FNV-1a 32-bit hash of the message name
  sequence:    uint32   // request/reply matching (unused here)
  instance_id: uint32   // request/reply matching (unused here)
  payload:     bytes    // CBOR-encoded message body
}
```

`src` and `msg_type` are **FNV-1a 32-bit hashes**, not small integers — e.g.
`msg_type = FNV("DW1000TagEvent")`. The GUI maps them back to names with
`id_to_string()`.

### 6.3 Message catalog

| Message            | `msg_type` hash of | Direction | Cadence | Payload |
|--------------------|--------------------|-----------|---------|---------|
| `DeviceEvent`      | announce           | device → server (multicast + unicast) | 5 s | `device_name`, `request_types`, `event_types` |
| `DeviceRequest`    | set-broker         | server → device | on demand | `ip_address`, `port`, `device_name` |
| `SysEvent`         | sys event          | device → server | 30 s | `uptime` |
| `DW1000AnchorEvent`| anchor identity    | anchor → server | boot + 30 s | `uwb_mac_address` (8-byte EUI) |
| `DW1000TagEvent`   | ranging report     | tag → server | 4 Hz per anchor | `uwb_mac_address`, `anchor` (short addr), `distance_m`, `rx_power_db` |

> **Wire-format note:** messages are **positional CBOR arrays** (fields in
> declaration order, absent fields `null`) — `msgs.cpp` emits `.beginArray(N)` and
> the Rust crate uses `#[cbor(array)]`. Two stale spots remain in the source: the
> header comment in `uwb_proto.h` still says "protobuf", and that file retains a
> dead hand-rolled protobuf writer/reader (the `UwbProto` class actually encodes
> through the generated CBOR classes). The generated `msgs.h`/`msgs.cpp` headers
> also say "CBOR maps keyed by field id" — wrong; the code emits arrays.

### 6.4 Defaults

- Server: `192.168.0.148:50000` (`-DUWB_SERVER_IP`, `-DUWB_PORT`).
- Multicast group: `239.0.0.23:50000` (`-DUWB_MULTICAST_IP`).
- These are injected per project via `platformio.ini` build flags.

## 7. Device identity

| Device | `DEVICE_NAME` | UWB EUI-64 | OTA hostname |
|--------|---------------|------------|--------------|
| ESP8266 tag | `uwb_tag_1` | `7D:00:22:EA:82:60:3B:9C` | `uwb-tag1` |
| ESP32 tag   | `uwb_tag_2` | `7D:00:22:EA:82:60:3B:9D` | `uwb-tag2` |
| anchor N    | `uwb_anchor_N` | `82:17:5B:D5:A9:9A:E2:<0x9B+N>` | `uwb-anchorN` |

Anchors derive name, EUI and OTA hostname from the `ANCHOR_ID` build flag; the
tag names and EUIs are hardcoded at the top of each tag sketch. The `src` hash
in every envelope is the FNV-1a hash of `DEVICE_NAME`.

## 8. Firmware structure

All three sketches share the same skeleton (`setup()` → `loop()`):

1. `setup()` — serial, LEDs, WiFi (`WIFI_STA`), OTA, then:
   - `proto.begin(DEVICE_NAME, caps, n)` opens the random-port UDP socket;
   - `DW1000Ranging.initCommunication(RST, SS, IRQ)`;
   - attach callbacks (`newRange`, `newBlink`, `inactiveDevice`);
   - `DW1000.setAntennaDelay(…)`;
   - `startAsTag(…)` or `startAsAnchor(…)` in `MODE_LONGDATA_RANGE_ACCURACY`.
2. `loop()` — non-blocking, and **order matters**:
   - LED heartbeat (2 Hz) + 1 ms UWB-activity pulse;
   - `DW1000Ranging.loop()` — services the DWM1000 state machine;
   - `ArduinoOTA.handle()`;
   - `proto.loop()` — drains incoming UDP (SetBroker);
   - periodic reporting (announce / sys-event / tag-event) only when
     `WL_CONNECTED`.

The `newRange()` callback runs in the DW1000 IRQ context and is kept minimal: it
reads the distant device's short address / range / RX power and updates the
`anchors[]` array with the filters from §5.5. Actual UDP sends happen in `loop()`.

## 9. DWM1000 library patches

The driver is a patched copy of `thotro/arduino-dw1000` tracked at
`libs/DW1000/`. Seven patches make it work on ESP8266/ESP32 and with 3+ anchors
(see `README.md` for the full diffs):

1. Put the IRQ handler in IRAM (`IRAM_ATTR`).
2. Same for the handler definition.
3. Hard-reset the chip **before** attaching the interrupt.
4. Slotted blink reply (16 random slots) so anchors don't collide on discovery.
5. Fixed short address from the last two EUI bytes.
6. ESP32-specific: custom SPI pins, drop `SPI.usingInterrupt()`, `INPUT_PULLUP`
   on RSTn, mark the whole IRQ call-chain `IRAM_ATTR`, disable HAL locks, 8 MHz
   SPI clock.
7. Multi-anchor ranging: reply-slot overflow fix, `LEN_DATA` 120 / `MAX_DEVICES`
   6, variable frame length, RANGE deadline, relaxed activity/inactivity.

`tag/` uses the unpatched `vortex314/arduino-dw1000#all-fixes` fork instead, so
Patch 7 is missing there; run multi-anchor setups on `tag_esp32/` + `anchor/`.

## 10. Central server (`gui/`)

The Rust egui app (`cargo run` in `gui/`) is the central sink:

- Binds `0.0.0.0:50000` and joins multicast group `239.0.0.23` in a dedicated
  thread (`udp_loop`), decoding every datagram with the generated crate.
- Decodes the `Envelope`, dispatches on `msg_type` into an `Event`, and pushes it
  over an `mpsc` channel to the UI thread.
- Maintains a `devices` map keyed by `src` (name, kind tag/anchor, IP, EUI,
  short address, uptime, last-seen) and a `ranges` map `tag → anchor-short →
  (distance, rx-power, last-seen)`.
- Derives each anchor's short address from its EUI (last two bytes,
  little-endian) to resolve anchor names, then renders a per-tag ranging grid.

## 11. Build, flash & OTA

```bash
cd tag        && pio run -t upload --upload-port /dev/ttyUSB0
cd tag_esp32  && pio run -t upload --upload-port /dev/ttyUSB0
cd anchor     && pio run -e anchor1 -t upload --upload-port /dev/ttyUSB1   # ..anchor5
cd gui        && cargo run
```

- WiFi credentials are **not** in the repo — inject via `WIFI_SSID` /
  `WIFI_PASS` (and optional `OTA_PASS`) environment variables at build time.
- PlatformIO Core 6.x; use `~/.platformio/penv/bin/pio` (apt `pio` 4.x breaks
  under Python 3.12).
- After first USB flash, update over the network:
  `pio run -t upload --upload-port uwb-tag2.local` (hostnames in §7).

## 12. Related docs

- [`../architecture.md`](../architecture.md) — the Limeros middleware this app builds on.
- [`README.md`](README.md) — patch diffs, pinouts, build/flash, calibration how-to.
- [`protocol.md`](protocol.md) — UDP protocol notes (CBOR section is accurate; multicast port shown there is stale).
- [`esp32_pinout_uext.md`](esp32_pinout_uext.md) — ESP32 wiring.
- `../coder/hcl/robot.hcl` — IDL from which `msgs.h` / `generated.rs` are emitted.
