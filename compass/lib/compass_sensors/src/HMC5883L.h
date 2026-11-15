#pragma once
// HMC5883L 3-axis digital magnetometer driver (Arduino TwoWire).
// Ported from the akkaEsp32 Compass component; register map and gain table
// follow the Honeywell HMC5883L datasheet.

#include <Arduino.h>
#include <Wire.h>

template <typename T>
struct Vec3 { T x, y, z; };

class HMC5883L {
public:
    static const uint8_t ADDRESS = 0x1E;

    enum Range : uint8_t {
        RANGE_0_88GA = 0b000,
        RANGE_1_3GA  = 0b001,
        RANGE_1_9GA  = 0b010,
        RANGE_2_5GA  = 0b011,
        RANGE_4GA    = 0b100,
        RANGE_4_7GA  = 0b101,
        RANGE_5_6GA  = 0b110,
        RANGE_8_1GA  = 0b111
    };

    enum Mode : uint8_t {
        CONTINUOUS = 0b00,
        SINGLE     = 0b01,
        IDLE       = 0b10
    };

    enum DataRate : uint8_t {
        RATE_0_75HZ = 0b000,
        RATE_1_5HZ  = 0b001,
        RATE_3HZ    = 0b010,
        RATE_7_5HZ  = 0b011,
        RATE_15HZ   = 0b100,
        RATE_30HZ   = 0b101,
        RATE_75HZ   = 0b110
    };

    enum Samples : uint8_t {
        SAMPLES_1 = 0b00,
        SAMPLES_2 = 0b01,
        SAMPLES_4 = 0b10,
        SAMPLES_8 = 0b11
    };

    explicit HMC5883L(TwoWire& wire, uint8_t addr = ADDRESS);

    bool begin();                   // probe + default config
    Vec3<int16_t> readRaw();        // raw LSB (X, Y, Z)
    Vec3<float> readNormalize();    // uT, offset applied

    void setOffset(int xo, int yo);
    void setRange(Range range);
    void setMode(Mode mode);
    void setDataRate(DataRate rate);
    void setSamples(Samples samples);

private:
    void    writeReg(uint8_t reg, uint8_t value);
    uint8_t readReg(uint8_t reg);
    int16_t readReg16(uint8_t reg);

    TwoWire& _wire;
    uint8_t  _addr;
    float    _uTPerDigit;
    int      _xOffset;
    int      _yOffset;
};
