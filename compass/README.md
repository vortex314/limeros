# Compass — ESP32 UEXT Compass + IMU

Arduino (PlatformIO) firmware that reads an **HMC5883L** magnetometer and an
**MPU6050** accel/gyro on the **UEXT1** I2C connector of an ESP32, computes a
tilt-compensated heading, and reports `CompassEvent` + `ImuEvent` over UDP using
the shared limeros CBOR transport (`libs/protocol/msgs.h`).

The sensor logic is ported from `akkaEsp32/components/DigitalCompass`
(HMC5883L + MPU6050 + Kalman pitch/roll fusion + tilt compensation).

## Layout

- `src/main.cpp` — sketch: WiFi/OTA, UEXT1 setup, 4 Hz measurement + UDP report
- `lib/uext` — C++ interface for UEXT1/UEXT2 (I2C/SPI/UART/GPIO)
- `lib/compass_sensors` — HMC5883L, MPU6050, Kalman, `CompassSensor` fusion
- `lib/compass_proto` — CBOR UDP transport (same pattern as the dwm1000 tag)

## Wiring (UEXT1, I2C)

- SCL — GPIO 25 (I2C clock, shared by HMC5883L + MPU6050)
- SDA — GPIO 26 (I2C data, shared by HMC5883L + MPU6050)

UEXT1/UEXT2 pin maps live in `lib/uext/src/uext.cpp` (from the akkaEsp32
`Hardware_ESP32.cpp` connector mapping):

- UEXT1: TXD=19 RXD=36 SCL=25 SDA=26 MISO=34 MOSI=23 SCK=17 CS=32
- UEXT2: TXD=18 RXD=39 SCL=27 SDA=14 MISO=35 MOSI=22 SCK=16 CS=33

UEXT1 I2C uses `Wire`, UEXT2 I2C uses `Wire1`.

## Build

```sh
export WIFI_SSID="your-ssid"
export WIFI_PASS="your-password"
export OTA_PASS="your-ota-password"   # optional
pio run -e esp32_compass -t upload
```

Server IP / multicast / port are overridable in `platformio.ini` build_flags:
`COMPASS_SERVER_IP`, `COMPASS_MULTICAST_IP`, `COMPASS_PORT`.

## Messages

- `CompassEvent` — heading (tilt-compensated, deg), pitch/roll (Kalman-filtered,
  deg), magnetometer (uT), accelerometer (m/s^2)
- `ImuEvent` — gyroscope (deg/s), accelerometer (m/s^2)
- `DeviceEvent` announce every 5 s and `SysEvent` every 30 s as liveness

The magnetometer axis remap inside `CompassSensor::read()` (`mag_x = Y`,
`mag_y = -X`) matches the akkaEsp32 board's chip orientation; adjust it if your
module is mounted differently.

## GUI

A live dashboard lives in `gui/` (Rust + egui). It binds UDP `0.0.0.0:50000` and
shows a compass rose, an attitude indicator with an azimuth ring, and numeric
IMU + magnetometer readouts.

```sh
cd gui
cargo run --release
```

The GUI reads the same UDP stream the `uwb_anchors` broker uses, so don't run
both on port 50000 at the same time.
