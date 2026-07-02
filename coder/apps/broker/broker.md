- Broker app routes udp messages from sources to destinations 
- keeps a list of subscriptions to filter incoming messages
- receives CBOR array encoded messages 
```rust 
struct UdpMessage {
    src : u32,
    dst : u32,
    msg_type : u32,
    req_id:u32,
    data : Vec<u8>,
}
```
- all fields in UdpMessage are optional and will be serialized as null if absent
- the broker listens to multicast udp messages where the payload is the EndpointAnnounce message
- the broker should record the endpoint_id (u32) for futher routing
- it reads the robot.hcl config and checks each endpoint for subscribes list, the fnv32 hash of the strings are the endpoints id's
- 
- it is based on tokio rust async
- any questions to clarify ?
- it uses the generated.rs as message structure source

