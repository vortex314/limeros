#include "HMC5883L.h"

// Register map (datasheet).
enum : uint8_t {
    REG_CONFIG_A = 0x00,
    REG_CONFIG_B = 0x01,
    REG_MODE     = 0x02,
    REG_OUT_X_M  = 0x03,
    REG_OUT_Z_M  = 0x05,
    REG_OUT_Y_M  = 0x07,
    REG_IDENT_A  = 0x0A,
    REG_IDENT_B  = 0x0B,
    REG_IDENT_C  = 0x0C
};

HMC5883L::HMC5883L(TwoWire& wire, uint8_t addr)
    : _wire(wire), _addr(addr), _uTPerDigit(0.092f), _xOffset(0), _yOffset(0) {}

bool HMC5883L::begin()
{
    _wire.beginTransmission(_addr);
    if (_wire.endTransmission() != 0) return false;  // device did not ACK

    if (readReg(REG_IDENT_A) != 0x48 ||
        readReg(REG_IDENT_B) != 0x34 ||
        readReg(REG_IDENT_C) != 0x33) {
        return false;
    }

    setRange(RANGE_1_3GA);
    setMode(CONTINUOUS);
    setDataRate(RATE_15HZ);
    setSamples(SAMPLES_1);
    return true;
}

Vec3<int16_t> HMC5883L::readRaw()
{
    Vec3<int16_t> v{0, 0, 0};
    _wire.beginTransmission(_addr);
    _wire.write(REG_OUT_X_M);
    _wire.endTransmission(false);
    if (_wire.requestFrom((int)_addr, 6) < 6) return v;

    uint8_t b[6];
    for (int i = 0; i < 6; i++) b[i] = (uint8_t)_wire.read();

    // HMC5883L output order is X, Z, Y.
    v.x = (int16_t)((b[0] << 8) | b[1]);
    v.z = (int16_t)((b[2] << 8) | b[3]);
    v.y = (int16_t)((b[4] << 8) | b[5]);
    return v;
}

Vec3<float> HMC5883L::readNormalize()
{
    // Single 6-byte burst read. The HMC5883L latches its output registers on
    // the first read and only releases them once register 0x08 (Y LSB) is read;
    // reading the axes piecemeal returns stale/frozen data.
    Vec3<int16_t> r = readRaw();
    Vec3<float> v;
    v.x = (float)(r.x - _xOffset) * _uTPerDigit;
    v.y = (float)(r.y - _yOffset) * _uTPerDigit;
    v.z = (float)r.z * _uTPerDigit;
    return v;
}

void HMC5883L::setOffset(int xo, int yo)
{
    _xOffset = xo;
    _yOffset = yo;
}

void HMC5883L::setRange(Range range)
{
    // Sensitivity per range (LSB per Gauss -> uT per digit).
    switch (range) {
        case RANGE_0_88GA: _uTPerDigit = 0.0073f; break;  // 1370 LSB/G
        case RANGE_1_3GA:  _uTPerDigit = 0.092f;  break;  // 1090 LSB/G
        case RANGE_1_9GA:  _uTPerDigit = 0.122f;  break;  //  820 LSB/G
        case RANGE_2_5GA:  _uTPerDigit = 0.152f;  break;  //  660 LSB/G
        case RANGE_4GA:    _uTPerDigit = 0.227f;  break;  //  440 LSB/G
        case RANGE_4_7GA:  _uTPerDigit = 0.256f;  break;  //  390 LSB/G
        case RANGE_5_6GA:  _uTPerDigit = 0.303f;  break;  //  330 LSB/G
        case RANGE_8_1GA:  _uTPerDigit = 0.435f;  break;  //  230 LSB/G
        default: break;
    }
    writeReg(REG_CONFIG_B, (uint8_t)(range << 5));
}

void HMC5883L::setMode(Mode mode)
{
    uint8_t v = readReg(REG_MODE);
    v &= 0b11111100;
    v |= (uint8_t)mode;
    writeReg(REG_MODE, v);
}

void HMC5883L::setDataRate(DataRate rate)
{
    uint8_t v = readReg(REG_CONFIG_A);
    v &= 0b11100011;
    v |= (uint8_t)(rate << 2);
    writeReg(REG_CONFIG_A, v);
}

void HMC5883L::setSamples(Samples samples)
{
    uint8_t v = readReg(REG_CONFIG_A);
    v &= 0b10011111;
    v |= (uint8_t)(samples << 5);
    writeReg(REG_CONFIG_A, v);
}

void HMC5883L::writeReg(uint8_t reg, uint8_t value)
{
    _wire.beginTransmission(_addr);
    _wire.write(reg);
    _wire.write(value);
    _wire.endTransmission();
}

uint8_t HMC5883L::readReg(uint8_t reg)
{
    _wire.beginTransmission(_addr);
    _wire.write(reg);
    _wire.endTransmission(false);
    if (_wire.requestFrom((int)_addr, 1) < 1) return 0;
    return (uint8_t)_wire.read();
}

int16_t HMC5883L::readReg16(uint8_t reg)
{
    _wire.beginTransmission(_addr);
    _wire.write(reg);
    _wire.endTransmission(false);
    if (_wire.requestFrom((int)_addr, 2) < 2) return 0;
    uint8_t hi = (uint8_t)_wire.read();
    uint8_t lo = (uint8_t)_wire.read();
    return (int16_t)((hi << 8) | lo);
}
