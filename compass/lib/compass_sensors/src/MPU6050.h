#pragma once
// MPU6050 6-axis IMU driver (Arduino TwoWire).
// Ported from the akkaEsp32 DigitalCompass component. Configured for
// ±2 g accelerometer and ±2000 deg/s gyroscope.

#include <Arduino.h>
#include <Wire.h>

class MPU6050 {
public:
    static const uint8_t ADDRESS = 0x68;

    explicit MPU6050(TwoWire& wire, uint8_t addr = ADDRESS);

    bool begin();   // wake up + configure ±2g / ±2000dps

    float accelX() const;   // m/s^2
    float accelY() const;
    float accelZ() const;

    float gyroX() const;    // deg/s
    float gyroY() const;
    float gyroZ() const;

    // Raw accelerometer output: 6 bytes (X, Y, Z big-endian int16).
    void readRawAccel(uint8_t out[6]);

private:
    void    writeReg(uint8_t reg, uint8_t value);
    int16_t readReg16(uint8_t reg) const;

    TwoWire& _wire;
    uint8_t  _addr;
    float _accelScale;  // LSB per 1 g
    float _gyroScale;   // LSB per 1 deg/s
};
