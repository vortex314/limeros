# UWB UDP protocol

CBOR-encoded messages over UDP. The single source of truth for the message
layout is `msgs.proto`; 
The fields are added in a CBOR array , they are sequential, and all are optional , if not present it is serialized as null 

- client devices take random udp ports
- each message contains all fields as optional , if not present they are null
- events dont have a dst field
- request/ reply have a src and dst field 
- msg_type determines how payload is being parsed
- use UDP to send events to a central server : default 192.168.0.121 : 50000
- use UDP multicast to a address 239.0.0.23:50001 to send the Announce message 
- listen to UDP incoming messages and parse payload for handling defined by msg_type
- an incoming message can be SetBroker to switch to another broker
- use the socket to send every 5 sec a announce message via multicast containing : device name
```

## msg_type

FNV32 hash of message naem as found 

## Transport

- Every device opens a UDP socket on a **random port**.
- **Announce** is multicast to `239.0.0.23:50000` *and* unicast to the server
  (`192.168.0.121:50000`), so the central server sees it even where multicast
  is not routed.
- **Events** (SysEvent, UwbAnchorEvent, UwbTagEvent) are unicast to the central
  server at `192.168.0.121:50000`.
- Events carry no `dst`; request/reply messages carry both `src` and `dst`.
- Every field is optional 
- `msg_type` determines how `payload` is parsed.
- A device listens for incoming UDP and acts on `msg_type`; a `SetBroker`
  switches its server address.
- Devices send an `Announce` every 5 s via the multicast 

## Conventions

- `src` = FNV-1a 32-bit hash of the device name (e.g. `uwb_anchor_1`).
- `msg_type`is fnv hash of the message name in msgs.proto
- `ip_address` fields are 4-byte IPv4 in network (big-endian) order.
- UWB MACs are 8-byte EUI-64; the tag's `anchor` field is the 2-byte responder
  short address in big-endian order (matches the `0x%04X` serial display).
- `wifi_rssi` is `int32` so negative dBm round-trips.
- `msg_types` in `Announce` is a packed `repeated uint32`.

