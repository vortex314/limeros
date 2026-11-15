#include "MPU6050.h"

enum : uint8_t {
    PWR_MGMT_1    = 0x6B,
    SMPLRT_DIV    = 0x19,
    CONFIG        = 0x1A,
    GYRO_CONFIG   = 0x1B,
    ACCEL_CONFIG  = 0x1C,
    ACCEL_XOUT_H  = 0x3B,
    GYRO_XOUT_H   = 0x43
};

MPU6050::MPU6050(TwoWire& wire, uint8_t addr)
    : _wire(wire), _addr(addr), _accelScale(16384.0f), _gyroScale(16.4f) {}

bool MPU6050::begin()
{
    _wire.beginTransmission(_addr);
    if (_wire.endTransmission() != 0) return false;  // device did not ACK

    // Wake up (clear sleep bit).
    writeReg(PWR_MGMT_1, 0x00);
    delay(10);
    // Sample rate divider -> gyro sample rate 1 kHz / (1 + 7) = 125 Hz.
    writeReg(SMPLRT_DIV, 0x07);
    // Digital low-pass filter (DLPF_CFG = 7 -> gyro bandwidth ~3.6 kHz).
    writeReg(CONFIG, 0x07);
    // Gyro full-scale ±2000 deg/s (FS_SEL = 3).
    writeReg(GYRO_CONFIG, 0x18);
    // Accel full-scale ±2 g (AFS_SEL = 0).
    writeReg(ACCEL_CONFIG, 0x01);
    return true;
}

float MPU6050::accelX() const
{
    return readReg16(ACCEL_XOUT_H) / _accelScale * 9.80665f;
}

float MPU6050::accelY() const
{
    return readReg16((uint8_t)(ACCEL_XOUT_H + 2)) / _accelScale * 9.80665f;
}

float MPU6050::accelZ() const
{
    return readReg16((uint8_t)(ACCEL_XOUT_H + 4)) / _accelScale * 9.80665f;
}

float MPU6050::gyroX() const
{
    return readReg16(GYRO_XOUT_H) / _gyroScale;
}

float MPU6050::gyroY() const
{
    return readReg16((uint8_t)(GYRO_XOUT_H + 2)) / _gyroScale;
}

float MPU6050::gyroZ() const
{
    return readReg16((uint8_t)(GYRO_XOUT_H + 4)) / _gyroScale;
}

void MPU6050::readRawAccel(uint8_t out[6])
{
    _wire.beginTransmission(_addr);
    _wire.write(ACCEL_XOUT_H);
    _wire.endTransmission(false);
    if (_wire.requestFrom((int)_addr, 6) < 6) {
        for (int i = 0; i < 6; i++) out[i] = 0;
        return;
    }
    for (int i = 0; i < 6; i++) out[i] = (uint8_t)_wire.read();
}

void MPU6050::writeReg(uint8_t reg, uint8_t value)
{
    _wire.beginTransmission(_addr);
    _wire.write(reg);
    _wire.write(value);
    _wire.endTransmission();
}

int16_t MPU6050::readReg16(uint8_t reg) const
{
    _wire.beginTransmission(_addr);
    _wire.write(reg);
    _wire.endTransmission(false);
    if (_wire.requestFrom((int)_addr, 2) < 2) return 0;
    uint8_t hi = (uint8_t)_wire.read();
    uint8_t lo = (uint8_t)_wire.read();
    return (int16_t)((hi << 8) | lo);
}
