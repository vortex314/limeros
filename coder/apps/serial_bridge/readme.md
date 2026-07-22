# serial_bridge

Serial bridge for the Limeros eventbus. Connects a Linux host to an ESP32 device over a UART serial link using the exact same framing protocol as the ESP32 firmware (`hoverboard_actor.cpp` / `codec.cpp`): **CBOR envelopes with CRC-16 and COBS framing**.

---

## What it does

- **Receives** `EndpointAnnounce` and `HoverboardEvent` messages from the ESP32 and displays them in human-readable form.
- **Sends** `HoverboardRequest` (speed/steer commands) and `EndpointAnnounceReply` (broker handshake replies) over the serial line.

No broker is needed — the host talks directly to the ESP32 eventbus through a USB-to-UART adapter (e.g. FTDI, CP2102, or the ESP32's built-in USB-serial-JTAG).

---

## Quick start

```bash
cd coder
cargo run -p serial_bridge -- --port /dev/ttyUSB0
```

Use `--baud` (default 115200) for non-standard rates:

```bash
cargo run -p serial_bridge -- --port /dev/ttyUSB0 --baud 9600
```

---

## Interactive commands

| Command | Effect |
|---|---|
| `hb <speed> <steer>` | Send a full HoverboardRequest (both fields) |
| `speed <value>` | Send HoverboardRequest with speed only |
| `steer <value>` | Send HoverboardRequest with steer only |
| `announce` | Send EndpointAnnounce (identifies this bridge to the ESP32) |
| `announce-reply` | Send EndpointAnnounceReply (broker handshake reply) |
| `quit` / `exit` | Stop the bridge |

**Values**: speed is motor RPM, steer is steering offset. Units depend on the hoverboard firmware configuration.

---

## Example session

```
$ serial_bridge --port /dev/ttyUSB0
serial_bridge listening on /dev/ttyUSB0 @ 115200 baud
Commands:
  speed <value>          — send HoverboardRequest with speed
  steer <value>          — send HoverboardRequest with steer
  hb <speed> <steer>     — send HoverboardRequest with both
  announce               — send EndpointAnnounce
  announce-reply         — send EndpointAnnounceReply
  quit / exit            — exit

⬅ EndpointAnnounce: esp1 (0x44B47F73) — ESP32 main board
⬅ HoverboardEvent  cmdl=300  cmdr=310  rpm_avg=305  batV=36.50  temp=32.1°C  dc_i=1.50A  (from hoverboard)
⬅ HoverboardEvent  cmdl=300  cmdr=310  rpm_avg=304  batV=36.48  temp=32.1°C  dc_i=1.48A  (from hoverboard)
hb 200 30
Sent HoverboardRequest speed=200 steer=30
⬅ HoverboardEvent  cmdl=200  cmdr=230  rpm_avg=215  batV=36.45  temp=32.2°C  dc_i=2.10A  (from hoverboard)
quit
Done.
```

---

## Wire protocol

Every frame on the serial line follows the same pipeline as `codec.cpp`:

```
SEND:  CBOR bytes → CRC-16 CCITT (big-endian) → COBS encode → 0x00 delimiter
RECV:  buffer until 0x00 → COBS decode → verify CRC-16 → CBOR decode
```

### Step by step

1. **CBOR encode** — the message (wrapped in an `Envelope`) is serialised with `cbor2`
2. **CRC-16 CCITT** — two CRC bytes are appended (polynomial `0x1021`, initial value `0xFFFF`, no reflection, no final XOR), big-endian
3. **COBS encode** — Consistent Overhead Byte Stuffing escapes any `0x00` bytes so the delimiter is unambiguous
4. **`0x00` delimiter** — appended as a frame terminator

This is the same wire format used by the ESP32 `FrameEncoder::add_crc()` / `FrameEncoder::add_cobs()` path and the hoverboard STM32 firmware's `get_txd()` output.

### Message structure

All messages are wrapped in an `Envelope`:

```rust
Envelope {
    src:       Option<u32>,   // FNV-1a hash of source endpoint name
    dst:       Option<u32>,   // destination (None = broadcast)
    msg_type:  Option<u32>,   // FNV-1a hash of message type name
    payload:   Option<Vec<u8>>, // CBOR-encoded inner message
    ...
}
```

The payload is then decoded into the specific message type (`HoverboardEvent`, `EndpointAnnounce`, etc.) based on `msg_type`.

---

## Displayed fields (HoverboardEvent)

| Field | Description | Scale |
|---|---|---|
| `cmdl` | Left motor command RPM | raw |
| `cmdr` | Right motor command RPM | raw |
| `rpm_avg` | Average measured RPM | raw |
| `batV` | Battery voltage | `× 100` → volts |
| `temp` | Temperature | `× 10` → °C |
| `dc_i` | Total DC link current | `× 100` → amps |

Additional fields are available in the `HoverboardEvent` struct (`spdl`, `spdr`, `ldc_curr`, `rdc_curr`, `ctrl_mod`, `ctrl_typ`, etc.) and can be added to the display handler in `handle_envelope()`.

---

## Project layout

```
coder/apps/serial_bridge/
├── Cargo.toml          # crate manifest
├── src/
│   ├── main.rs         # CLI, serial I/O, message display/send
│   └── codec.rs        # CRC-16 CCITT, COBS encode/decode, frame roundtrip
└── readme.md           # this file
```

---

## Building

From the `coder/` workspace root:

```bash
cargo build -p serial_bridge
cargo test -p serial_bridge
```

---

## Hardware setup

```
  Linux PC                          ESP32
  ┌──────────┐                   ┌──────────┐
  │ USB port │─── FTDI adapter ──│ UART2    │
  │          │   TX ──────────→  │ GPIO16   │ (RX)
  │          │   RX ←──────────  │ GPIO17   │ (TX)
  │          │   GND ──────────  │ GND      │
  └──────────┘                   └──────────┘
```

- **Baud rate**: 115200 (default, matches `UART_BAUD_RATE` in `hoverboard_actor.cpp`)
- **Data bits**: 8
- **Parity**: none
- **Stop bits**: 1
- **Flow control**: none
