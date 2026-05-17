# Limeros — Robotic Middleware Architecture

## Overview

Limeros is a lightweight robotic middleware targeting low-latency communication between
microcontrollers (ESP32) and Linux systems. It supports two communication patterns —
publish/subscribe and request/reply — over UDP, with peer-to-peer operation and no
mandatory central broker.

## Features

- Low latency based on UDP messages
- Central IDL: `coder/syntax/message.proto` — protobuf-like schema defining all messages
- Compile-time serialization choice: JSON (development) or CBOR (production)
- Publish/subscribe for events, request/reply for commands, both peer-to-peer
- Actor-based in-process eventbus on both ESP32 (FreeRTOS) and Linux (Tokio)
- Code generation from the IDL for C++ and Rust via Tera templates

---

## 1. Central IDL

All message types are defined in a single protobuf-like schema at `coder/syntax/message.proto`.
The `coder/` Rust crate generates **C++ headers** and **Rust structs** from this file using
Tera templates, keeping the ESP32 and Linux sides in lockstep.

Generated message types span:

| Domain | Messages |
|--------|----------|
| System | `SysEvent`, `SysRequest`, `SysReply`, `LogEvent`, `WifiEvent` |
| Hoverboard | `HoverboardEvent`, `HoverboardRequest`, `HoverboardReply` |
| PS4 Controller | `Ps4Event`, `Ps4Request` |
| Camera | `CameraEvent`, `CameraRequest`, `CameraReply` |
| Lawnmower | `LawnmowerManualEvent/Request/Reply`, `LawnmowerAutoEvent/Request`, `LawnmowerStatus` |
| Sensors | `Max31855Event` (thermocouple), `HeatingEvent/Request` (PID heater), `MotorEvent` |
| Infrastructure | `AliveEvent`, `PingRequest`, `PingReply`, `Poly` (polymorphic envelope) |

All fields are `optional` (`Option<T>` in Rust, `std::optional` in C++) for backward
compatibility. Each generated message type carries a unique `u32` type ID and a string
`MSG_TYPE` constant.

---

## 2. Wire Format

Messages are wrapped in an envelope: `UdpMessage` (JSON) or `UdpMessageCbor` (CBOR).

| Field | Event (pub/sub) | Request/Reply |
|-------|-----------------|---------------|
| `src` | filled | filled |
| `dst` | **absent** | filled |
| `typ` | message type string | message type string |
| `msg` | serialized payload | serialized payload |

- **Events**: `dst` is null → broadcast to all subscribers.
- **Request/Reply**: `src` and `dst` are both filled → unicast UDP.
- Serialization is compile-time selectable — JSON during development for readability,
  CBOR for production to minimize wire size.

### Topic Naming Convention

Topics follow the pattern: `{direction}/{device}/{component}/{message_type}`

Examples:
- `src/esp1/hb/HoverboardCmd`
- `src/esp1/sys/SysInfo`

This allows subscriptions at any granularity:
- `src/esp1/*` — all messages from device `esp1`
- `src/*/HoverboardEvent` — hoverboard events from any device

---

## 3. Communication Patterns

### Discovery / Announce

Each device multicasts an `AliveEvent` roughly every 5 seconds, advertising:
- `subscribes`: topics it consumes
- `publishes`: topics it produces
- `services`: request/reply endpoints it provides

### Publish/Subscribe

Events flow from producer to all interested consumers. Devices are discovered via mDNS
or configured with hardcoded IPs.

### Request/Reply

Point-to-point unicast UDP between caller and callee. Each request type has a matching
reply type:

- `PingRequest` → `PingReply`
- `SysRequest` → `SysReply`
- `HoverboardRequest` → `HoverboardReply`
- `CameraRequest` → `CameraReply`
- `HeatingRequest` → `HeatingEvent`
- `LawnmowerManualRequest` → `LawnmowerManualReply`

---

## 4. Transport Strategy

Real-world WiFi constraints drive the transport design (from `esp32-eventbus/multicast.md`):

| Mode | PHY Rate | Usage |
|------|----------|-------|
| Multicast | ~1 Mbps (lowest rate) | Scouting, discovery, non-time-critical events |
| Unicast | 54 Mbps (full rate) | Events and request/reply to logical devices |

Events go to a logical device (resolved via mDNS or hardcoded IP), not to a multicast
group. Multicast is reserved for discovery to avoid bandwidth contention.

---

## 5. In-Process EventBus

Both platforms share the same actor-bus pattern.

### Rust (`rust/src/eventbus.rs`)

- Single-threaded Tokio `Eventbus` with `UnboundedSender`/`UnboundedReceiver`.
- `ActorImpl` trait: `async fn handle(&mut self, msg: &Arc<dyn Any + Send + Sync>)`.
- Actors receive lifecycle messages (`ActorStart`, `ActorStop`, `ActorTick`) plus typed
  domain messages.
- `on_message<T>(msg, handler)` helper dispatches by downcasting to the concrete type.
- The eventbus fans every message to **all** registered actors; each actor decides
  whether to act on a given type.

### C++ / ESP32 (`esp32-eventbus/include/actor.h`)

- `EventBus` extends `Queue<const Envelope*>` — a FreeRTOS queue of heap-allocated
  message pointers.
- Each actor runs in its own FreeRTOS task with a dedicated queue.
- `QueueSet` multiplexes multiple queues for single-threaded select/poll.
- `Timer`/`Timers` classes for one-shot and repetitive timers.
- `Envelope` carries optional `src`/`dst` `ActorRef` and a `Msg*`.
- `ActorRef` matches by string name; `NULL_ACTOR` sentinel for unset references.
- Messages are dispatched via `handle<T>(msg, callback)` template, matching on
  `T::id` (the string type identifier).

---

## 6. Actor Inventory

### ESP32 Actors (`esp32-eventbus/src/`)

| Actor | Role |
|-------|------|
| `SysActor` | System info, uptime, free heap, flash, reboot |
| `WifiActor` | WiFi status, SSID, BSSID, RSSI, IP, MAC |
| `HoverboardActor` | Motor control, telemetry, FOC/commutation |
| `LedActor` | Onboard LED control |
| `Max31855Actor` | Thermocouple temperature sensor |
| `McActor` | Motor controller abstraction |
| `MdnsActor` | mDNS service advertisement and discovery |
| `ZenohActor` | Zenoh protocol integration (optional layer) |
| `OtaActor` | Over-the-air firmware updates |

### Rust Binaries (`rust/src/bin/`)

| Binary | Purpose |
|--------|---------|
| `broker` | Central UDP message router |
| `tui` | Terminal-based dashboard |
| `egui` | Graphical dashboard — plots, gauges, heater, hoverboard |
| `ticktock_udp` | UDP event logger |
| `client` | CLI client for sending commands |
| `wv` | WebView-based UI |

---

## 7. Design Principles

- **No single point of failure** — devices communicate peer-to-peer; the broker is optional.
- **Component-granularity messages** — sending one field vs. all fields costs roughly the
  same CPU, so messages carry full component state rather than single-field updates.
- **Optional fields for backward compatibility** — all fields are `optional`/`Option<T>`,
  allowing old and new firmware to interoperate.
- **String-based type IDs** for readability during development; a future optimization
  path uses `u32` hashes for topics and `u16` hashes for field names.
- **Alive detection via data messages** — no separate heartbeat protocol; frequent data
  messages from each component serve as liveness indicators.
- **Code generation from a single IDL** — the `coder/` crate reads `message.proto` and
  emits Rust structs and C++ classes with serialization/deserialization, type IDs, and
  the `Msg` trait implementation. Tera templates make adding new target languages or
  serialization formats straightforward.

---

## 8. Project Structure

```
limeros/
├── architecture.md          ← this file
├── coder/                   # Code generator (Rust)
│   ├── syntax/message.proto # Central IDL
│   ├── src/*.tera           # Tera templates for C++ and Rust
│   └── src/bin/             # Parser, logger, mermaid diagram generator
├── esp32-eventbus/          # ESP32 firmware (C++, FreeRTOS)
│   ├── include/             # Headers: actor.h, msg.h, serdes.h, ...
│   ├── src/                 # Actor implementations
│   └── platformio.ini       # PlatformIO build config
├── rust/                    # Linux/desktop side (Rust, Tokio)
│   ├── src/
│   │   ├── eventbus.rs      # In-process actor bus
│   │   ├── msgs.rs          # Generated message types
│   │   ├── poly.rs          # Polymorphic serialization
│   │   └── bin/             # Binaries: broker, tui, egui, client, ...
│   └── Cargo.toml
└── arduino/                 # Arduino-compatible variant
    └── src/main.cpp
```
