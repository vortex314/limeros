#include "compass.h"
#include <math.h>

namespace {
constexpr float kRadToDeg = 57.2957795131f;
constexpr float kDegToRad = 0.01745329252f;
constexpr float kTwoPi    = 6.28318530718f;
// Low-pass coefficient for the heading inputs (magnetometer + tilt angles).
// At 4 Hz this gives a ~0.8 s time constant: smooths hand tremor/vibration but
// still tracks real rotations.
constexpr float kFilterAlpha = 0.3f;
}

CompassSensor::CompassSensor(Uext& uext, float dt)
    : _uext(uext), _hmc(nullptr), _mpu(nullptr),
      _pfilter(dt), _rfilter(dt),
      _mag{0.0f, 0.0f, 0.0f},
      _ax(0), _ay(0), _az(0), _gx(0), _gy(0), _gz(0),
      _pitchDeg(0), _rollDeg(0), _fpitch(0), _froll(0),
      _pitchFilt(0), _rollFilt(0), _magFilt{0.0f, 0.0f, 0.0f},
      _heading(0) {}

CompassSensor::~CompassSensor()
{
    delete _hmc;
    delete _mpu;
}

bool CompassSensor::begin()
{
    TwoWire& wire = _uext.i2c();
    _hmc = new HMC5883L(wire);
    _mpu = new MPU6050(wire);

    bool ok = true;
    if (!_mpu->begin()) {
        Serial.println("MPU6050: init failed");
        ok = false;
    }
    if (!_hmc->begin()) {
        Serial.println("HMC5883L: init failed");
        ok = false;
    }
    if (ok) {
        _hmc->setRange(HMC5883L::RANGE_1_3GA);
        _hmc->setMode(HMC5883L::CONTINUOUS);
        _hmc->setDataRate(HMC5883L::RATE_75HZ);
        _hmc->setSamples(HMC5883L::SAMPLES_8);
        _hmc->setOffset(0, 0);
    }
    return ok;
}

void CompassSensor::read()
{
    // Magnetometer in uT (sensor axes).
    _mag = _hmc->readNormalize();

    // Low-pass the magnetometer so noise does not feed the heading.
    _magFilt.x += kFilterAlpha * (_mag.x - _magFilt.x);
    _magFilt.y += kFilterAlpha * (_mag.y - _magFilt.y);
    _magFilt.z += kFilterAlpha * (_mag.z - _magFilt.z);

    // Scaled accelerometer (m/s^2) and gyroscope (deg/s).
    _ax = _mpu->accelX();
    _ay = _mpu->accelY();
    _az = _mpu->accelZ();
    _gx = _mpu->gyroX();
    _gy = _mpu->gyroY();
    _gz = _mpu->gyroZ();

    // Raw accelerometer LSB (matches the akkaEsp32 calc() tilt math).
    uint8_t raw[6];
    _mpu->readRawAccel(raw);
    int16_t rax = (int16_t)((raw[0] << 8) | raw[1]);
    int16_t ray = (int16_t)((raw[2] << 8) | raw[3]);
    int16_t raz = (int16_t)((raw[4] << 8) | raw[5]);

    // Tilt angles from the accelerometer (radians -> degrees).
    // Roll sign is negated so the tilt compensation cancels roll instead of
    // adding to it; pitch keeps the standard sign.
    _pitchDeg = atan2f((float)-rax, sqrtf((float)raz * raz + (float)ray * ray)) * kRadToDeg;
    _rollDeg  = atan2f((float)-ray, sqrtf((float)raz * raz + (float)rax * rax)) * kRadToDeg;

    // Low-pass the tilt angles used for tilt compensation so motion / vibration
    // does not make the heading wander.
    _pitchFilt += kFilterAlpha * (_pitchDeg - _pitchFilt);
    _rollFilt  += kFilterAlpha * (_rollDeg - _rollFilt);

    // Kalman pitch/roll fusion (accel angle + gyro rate), as in DigitalCompass:
    // gyro Y feeds pitch, gyro X (negated) feeds roll.
    float pitchSimple = atan2f(-_ax, _az) * kRadToDeg;
    float rollSimple  = atan2f(_ay, _az) * kRadToDeg;
    _fpitch = _pfilter.filter(pitchSimple, _gy);
    _froll  = _rfilter.filter(rollSimple, -_gx);

    // Magnetometer axes remapped to the board frame: the HMC5883L is mounted
    // 90° rotated relative to the MPU6050 on this module.
    float mx = _magFilt.y;
    float my = -_magFilt.x;
    float mz = _magFilt.z;

    // Tilt compensation:
    //   Xh = X*cos(p) + Y*sin(r)*sin(p) + Z*cos(r)*sin(p)
    //   Yh = Y*cos(r) - Z*sin(r)
    float cp = cosf(_pitchFilt * kDegToRad);
    float sp = sinf(_pitchFilt * kDegToRad);
    float cr = cosf(_rollFilt * kDegToRad);
    float sr = sinf(_rollFilt * kDegToRad);

    float Xh = mx * cp + my * sr * sp + mz * cr * sp;
    float Yh = my * cr - mz * sr;

    float azimuth = atan2f(Yh, Xh);
    if (azimuth < 0.0f) azimuth += kTwoPi;
    _heading = azimuth * kRadToDeg;
}
