# UWB UDP protocol

CBOR-encoded messages over UDP. 
The fields are added in a CBOR array , they are sequential, and all are optional , if not present it is serialized as null , the declaration order is maintained. 
An incomplete array is also considered as filled with nulls.

- client devices take random udp ports
- each message contains all fields as optional , if not present they are null
- events dont have a dst field
- request/ reply have a src and dst field 
- msg_type determines how payload is being parsed
- use UDP to send events to a central server : default 192.168.0.121 : 50000
- use UDP multicast to a address 239.255.0.23:50001 to send the Announce message, the socket for the unicast UDP is also used for this.  
- listen to UDP incoming messages and parse payload for handling defined by msg_type
- an incoming message can be SetBroker to switch to another broker
- use the socket to send every 5 sec a announce message via multicast containing : device name and supported msg types generated and consumed
- the central server or broker has the capability to support subscribe on src,dst and msg_type
- there is no retry or check on reception, 
- the client doesnt listen to the multicast address
- there is no further security added as it is completely intern to a robot

```


```proto
message Envelope {
    uint32 src ;      // FNV-1a 32-bit hash of the device name
    uint32 dst ;      // null
    uint32 msg_type ; // determines how payload is parsed
    bytes  payload ;
}
```

## msg_type

FNV32 hash of message name as found in the description 

## Transport

- Every device opens a UDP socket on a **random port**.
- **Announce** is multicast to `239.0.0.23:50000` 
- **Events** (SysEvent, UwbAnchorEvent, UwbTagEvent) are unicast to the central
  server at `192.168.0.121:50000`.
- Events carry no `dst`; request/reply messages carry both `src` and `dst`.
- Request and replies are recognized by the endpôint based on message type
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

## Messages

```proto
message Announce {
    string         device     ; // e.g. "uwb_anchor_1"
    repeated uint32 req_msg_types ; // packed capabilities
    repeated uint32 evt_msg_types ;
    bytes          ip_address ; // 4 bytes
    uint32         port       ;
}

message SetBroker {
    bytes  ip_address ;
    uint32 port       ;
}

message Subscribe {

}

message SysEvent {
    uint64 uptime_ms        ;
    bytes  wifi_mac_address  // 6 bytes
    bytes  wifi_ip_address  ; // 4 bytes
    int32  wifi_rssi        ;
}

message UwbAnchorEvent {
    bytes uwb_mac_address ; // 8 bytes
}

message UwbTagEvent {
    bytes  uwb_mac_address ; // 8 bytes (tag)
    bytes  anchor          ; // 2 bytes (responder short address)
    float  distance_m      ;
    float  rx_power_db     ;
}
```
