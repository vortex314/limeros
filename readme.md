# Limeros — Lightweight Robotic Middleware

**Limeros** is a lightweight robotic middleware that connects microcontrollers (ESP32) and Linux systems with low-latency UDP messaging. It supports publish/subscribe and request/reply patterns, operates peer-to-peer with no mandatory central broker, and keeps both sides in lockstep through code generation from a single message schema.

---

## Why Limeros?

Most robotic middleware assumes big CPUs and big stacks. Limeros is built for the little guys — ESP32-class microcontrollers running FreeRTOS — while still giving your Linux brain (Rust, Tokio) the structured messaging it needs.

- **Zero-config discovery** — devices announce themselves via UDP multicast. No hardcoded IPs.
- **Peer-to-peer by default** — devices talk directly. No single point of failure. A broker is available when you want it.
- **Schema-driven** — all messages are defined in one protobuf-like IDL. Code generation produces C++ and Rust structs that always agree.
- **Minimal wire overhead** — CBOR serialization for production, JSON for development debugging.
- **Actor-based in-process eventbus** — same pattern on ESP32 (FreeRTOS tasks) and Linux (Tokio actors).

---

## Architecture at a Glance

```
┌──────────────────────────────────────────────────┐
│                  message.proto                    │  ◄── Single IDL
│         (protobuf-like schema for ALL messages)   │
└──────────────┬───────────────────────────────────┘
               │  codegen (Tera templates)
       ┌───────┴───────┐
       ▼               ▼
┌──────────────┐ ┌──────────────┐
│  C++ headers │ │  Rust structs│
│  (ESP32)     │ │  (Linux)     │
└──────┬───────┘ └──────┬───────┘
       │                │
   FreeRTOS          Tokio async
   Actors            Actors
       │                │
       └───────┬────────┘
               │  UDP (unicast + multicast)
         ┌─────┴─────┐
         │   WiFi     │
         └───────────┘
```

### Communication Patterns

| Pattern | Use case | Wire format |
|---------|----------|-------------|
| **Publish/Subscribe** | Telemetry, sensor events, state broadcasts | `dst` is null → all subscribers |
| **Request/Reply** | Commands, queries, configuration | `src` and `dst` both filled → unicast |
| **Discovery** | Finding peers and services | Multicast `AliveEvent` every ~5s |

### Topic Naming

```
{direction}/{device}/{component}/{message_type}
```

Examples: `src/esp1/hb/HoverboardCmd`, `src/esp1/sys/SysInfo`

Subscribe at any granularity: `src/esp1/*` catches everything from device `esp1`.

---

## Project Structure

```
limeros/
├── architecture.md               ← Full architecture reference
├── coder/                        ← Code generator (Rust)
│   ├── hcl/
│   │   ├── robot.hcl             # Device/endpoint configuration
│   │   ├── cpp.tera              # C++ codegen template
│   │   └── rust.tera             # Rust codegen template
│   ├── apps/
│   │   ├── codegen/              # IDL parser → generated code
│   │   ├── broker/               # Central UDP message router
│   │   ├── pinger/               # Network reachability tester
│   │   └── sniffer/              # Packet capture & inspection
│   ├── common/                   # Shared types, config, logging, FNV hashing
│   └── generated/                # Generated message types
├── esp32-eventbus/               ← ESP32 firmware (C++, FreeRTOS)
│   ├── include/                  # Actor, message, serialization headers
│   ├── src/                      # Actor implementations
│   └── platformio.ini            # PlatformIO build config
├── rust/                         ← Linux/desktop side (Rust, Tokio)
│   ├── src/
│   │   ├── eventbus.rs           # In-process actor bus
│   │   ├── msgs.rs               # Generated message types
│   │   ├── poly.rs               # Polymorphic JSON/CBOR serialization
│   │   └── bin/
│   │       ├── broker.rs         # Central message router
│   │       ├── tui.rs            # Terminal dashboard
│   │       ├── egui/             # Graphical dashboard (plots, gauges)
│   │       ├── ticktock_udp.rs   # UDP event logger
│   │       └── client.rs         # CLI command sender
│   └── Cargo.toml
└── arduino/                      ← Arduino-compatible variant
    └── src/main.cpp
```

---

## Message Domains

The IDL covers a full robot's worth of messages:

| Domain | Messages |
|--------|----------|
| **System** | `SysEvent`, `SysRequest`, `SysReply`, `LogEvent`, `WifiEvent` |
| **Hoverboard** | `HoverboardEvent`, `HoverboardRequest`, `HoverboardReply` |
| **PS4 Controller** | `Ps4Event`, `Ps4Request` |
| **Camera** | `CameraEvent`, `CameraRequest`, `CameraReply` |
| **Lawnmower** | `LawnmowerManualEvent/Request/Reply`, `LawnmowerAutoEvent/Request`, `LawnmowerStatus` |
| **Sensors** | `Max31855Event` (thermocouple), `HeatingEvent/Request` (PID heater), `MotorEvent` |
| **Infrastructure** | `AliveEvent`, `PingRequest`, `PingReply`, `Poly` (polymorphic envelope) |

All fields are optional (`Option<T>` / `std::optional`) for backward compatibility between firmware versions.

---

## ESP32 Actor Inventory

Each actor runs in its own FreeRTOS task:

| Actor | Role |
|-------|------|
| `SysActor` | System info, uptime, free heap, flash, reboot |
| `WifiActor` | WiFi status, SSID, BSSID, RSSI, IP, MAC |
| `HoverboardActor` | Motor control, telemetry, FOC/commutation |
| `LedActor` | Onboard LED control |
| `Max31855Actor` | Thermocouple temperature sensor |
| `McActor` | Motor controller abstraction |
| `MdnsActor` | mDNS service advertisement and discovery |
| `OtaActor` | Over-the-air firmware updates |
| `ZenohActor` | Optional Zenoh protocol integration |

---

## Transport Strategy

Real-world WiFi constraints drive every design decision:

| Mode | PHY Rate | Usage |
|------|----------|-------|
| **Multicast** | ~1 Mbps (lowest rate) | Discovery only — `AliveEvent` announcements |
| **Unicast** | 54 Mbps (full rate) | All events and request/reply traffic |

Events go to a logical device (resolved via mDNS or configuration), not a multicast group. Multicast is strictly reserved for discovery to avoid bandwidth contention.

---

## Design Principles

- **No single point of failure** — devices communicate peer-to-peer; the broker is optional.
- **Component-granularity messages** — sending one field costs roughly the same CPU as sending all fields, so messages carry full component state.
- **Optional fields everywhere** — old and new firmware interoperate without coordination.
- **String-based type IDs** for readability; future optimization path uses `u32` topic hashes and `u16` field hashes.
- **Alive detection via data messages** — no separate heartbeat protocol; frequent data messages serve as liveness indicators.
- **Code generation from a single IDL** — the `coder/` crate reads the schema and emits Rust + C++ with serialization, type IDs, and the `Msg` trait.

---

## Getting Started

### Prerequisites

- **Rust** (stable) for the Linux side and code generator
- **PlatformIO** for ESP32 firmware builds
- **Python 3** (ESP32 toolchain)

### Build the Code Generator

```bash
cd coder
cargo build --release
```

### Build ESP32 Firmware

```bash
cd esp32-eventbus
pio run
```

### Build Linux Binaries

```bash
cd rust
cargo build --release
```

### Run the Broker

```bash
cd rust
cargo run --release --bin broker
```

### Run the TUI Dashboard

```bash
cd rust
cargo run --release --bin tui
```

---

## License

Dual-licensed under either of:

- Apache License, Version 2.0 ([LICENSE-APACHE](LICENSE-APACHE))
- MIT license ([LICENSE-MIT](LICENSE-MIT))

at your option.
