# UWB UDP protocol

CBOR-encoded messages over UDP. Message types and their layouts are generated
from `coder/hcl/robot.hcl` into two targets that stay in lockstep:

- `libs/protocol/src/msgs.{h,cpp}` — C++ (TinyCBOR), used by the firmware
- `coder/generated/src/generated.rs` — Rust (cbor2), used by `gui/`

Both sides encode each message as a **positional CBOR array**: fields are
written in declaration order, and an absent field is serialized as `null`
(`0xf6`).

## Envelope

Every datagram is an `Envelope`:

```
Envelope {
  src:         uint32   // FNV-1a 32-bit hash of the device name
  dst:         uint32   // null for events, set for request/reply
  msg_type:    uint32   // FNV-1a 32-bit hash of the message name
  sequence:    uint32   // request/reply matching (unused in the UWB app)
  instance_id: uint32   // request/reply matching (unused in the UWB app)
  payload:     bytes    // CBOR-encoded message body
}
```

## msg_type

`msg_type` is the FNV-1a 32-bit hash of the message name, e.g.
`FNV("DW1000TagEvent")` — it selects how `payload` is parsed. The name tables in
`msgs.cpp` and `generated.rs` map the hash back to a readable name.

## Messages

| Message            | Direction        | Cadence         | Payload |
|--------------------|------------------|-----------------|---------|
| `DeviceEvent`      | device → server  | 5 s             | `device_name`, `request_types`, `event_types` |
| `DeviceRequest`    | server → device  | on demand       | `ip_address`, `port`, `device_name` |
| `SysEvent`         | device → server  | 30 s            | `uptime` |
| `DW1000AnchorEvent`| anchor → server  | boot + 30 s     | `uwb_mac_address` (8-byte EUI) |
| `DW1000TagEvent`   | tag → server     | 4 Hz per anchor | `uwb_mac_address`, `anchor`, `distance_m`, `rx_power_db` |

## Transport

- Every device opens a UDP socket on a **random port** (20000–60000).
- **Announce** (`DeviceEvent`) is sent every 5 s, multicast to
  `239.0.0.23:50000` *and* unicast to the server, so the central server sees it
  even where multicast is not routed. (ESP32 skips the multicast send.)
- **Events** (`SysEvent`, `DW1000AnchorEvent`, `DW1000TagEvent`) are unicast to
  the central server at `192.168.0.148:50000`.
- Events carry no `dst`; request/reply messages carry both `src` and `dst`.
- A device listens for incoming UDP; a `DeviceRequest` carrying `ip_address` +
  `port` switches its server address (SetBroker).

## Conventions

- `src` = FNV-1a 32-bit hash of the device name (e.g. `uwb_anchor_1`).
- `msg_type` = FNV-1a 32-bit hash of the message name.
- `ip_address` fields are 4-byte IPv4 in network (big-endian) order.
- UWB MACs are 8-byte EUI-64. The tag's `anchor` field is the responder's 16-bit
  short address — the last two EUI bytes with `mac[6]` least-significant
  (`…:E2:9C` → `0x9CE2`) — matching the `0x%04X` serial display.
- Repeated fields (`request_types`, `event_types`) are CBOR arrays of `uint32`.
