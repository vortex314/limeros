# Run ticktockdb in portainer

## increase nbr of file limits

- Edit /etc/docker/daemon.json
```json
{
    "default-ulimits": {
        "nofile": {
        "name": "nofile",
        "soft": 64000,
        "hard": 64000
        }
    }
}
```

## activate UDP in ticktockdb listener
- Edit /var/lib/ticktock/conf/ticktock.conf
/opt/ticktock/conf/ticktock.conf


```
udp.server.enabled = true

```
- log /var/lib/ticktock/log/ticktock.log


