# Multicast Alive on port 10000

```json
{
    "src":"heater",
    "typ":"Alive",
    "msg":{
        "services":["HeaterRequest"],
        "publishes":["HeaterEvent"],
        "subscribes":[]
    }
}
```

The multicast listener accepts the Alive messages for the services it is interested.
It records the IP and ports UDP associated with the 'src'. 
So when it sends a message to 'dst' it finds the UDP IP and port
Any client binds to a a random assigned port.

# Unicast Publish 
```json
{
    "src":"heater",
    "dst":"broker",
    "typ":"HeaterEvent",
    "msg":{
        "target_temp":150.0,
        "Kp":5.0,
        "Ki":0.01
    }
}
```

# Unicast request reply 
```json
{
    "src":"app",
    "dst":"heater",
    "typ":"HeaterRequest",
    "msg":{
        "target_temp":150.0,
        "Kp":5.0,
        "Ki":0.01
    }
}

{
    "src":"heater",
    "dst":"app",
    "typ":"HeaterReply",
    "msg":{
        "rc":0,
        "txt":"OK"
    }
}

```
