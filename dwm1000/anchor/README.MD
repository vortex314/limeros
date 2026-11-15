1. Identify boards by their chip MAC (most robust)
Each ESP8266 has a fixed factory MAC, and it travels with the board no matter which port or cable it's on. We already have the mapping from earlier:

┌─────────────┬───────────────────┐
│     env     │     chip MAC      │
├─────────────┼───────────────────┤
│ anchor1     │ 5c:cf:7f:15:a2:81 │
├─────────────┼───────────────────┤
│ anchor2     │ 18:fe:34:f3:a1:7c │
├─────────────┼───────────────────┤
│ anchor3     │ 18:fe:34:f4:b6:2e │
├─────────────┼───────────────────┤
│ anchor4     │ 18:fe:34:e8:2b:15 │
├─────────────┼───────────────────┤
│ anchor5     │ 18:fe:34:cf:52:41 │
├─────────────┼───────────────────┤
│ tag (ESP32) │ 30:ae:a4:ff:37:08 │
└─────────────┴───────────────────┘

A small flash script can run esptool.py read_mac on every port and look up the env for each MAC. It then flashes pio run -e anchorN -t upload --upload-port <that port> and refuses to flash any port whose MAC is unknown. That's the check I did by hand before the second flash round. It costs one reset per board, which is harmless.