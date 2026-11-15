#pragma once
// Tilt-compensated compass: HMC5883L magnetometer + MPU6050 (accel + gyro),
// ported from the akkaEsp32 DigitalCompass component.
//
//   - pitch/roll are fused with a 1-axis Kalman filter (accel angle + gyro rate)
//   - heading is the magnetometer azimuth tilt-compensated with pitch/roll
//
// Construct with a Uext (the sensors share its I2C bus); hardware is only
// touched inside begin(), so a global instance is safe.

#include <Arduino.h>
#include <uext.h>
#include "HMC5883L.h"
#include "MPU6050.h"
#include "Kalman.h"

class CompassSensor {
public:
    // dt = sensor sample period in seconds (default 0.25 s = 4 Hz).
    explicit CompassSensor(Uext& uext, float dt = 0.25f);
    ~CompassSensor();

    bool begin();   // configure HMC5883L + MPU6050 (call once in setup())
    void read();    // sample both chips, update filters and heading

    float heading()  const { return _heading;  }  // tilt-compensated, 0..360 deg
    float pitch()    const { return _fpitch;   }  // Kalman-filtered pitch, deg
    float roll()     const { return _froll;    }  // Kalman-filtered roll, deg
    float rawPitch() const { return _pitchDeg; }  // raw accel pitch, deg
    float rawRoll()  const { return _rollDeg;  }  // raw accel roll, deg

    float magX() const { return _mag.x; }  // uT
    float magY() const { return _mag.y; }
    float magZ() const { return _mag.z; }
    float accelX() const { return _ax; }   // m/s^2
    float accelY() const { return _ay; }
    float accelZ() const { return _az; }
    float gyroX() const { return _gx; }    // deg/s
    float gyroY() const { return _gy; }
    float gyroZ() const { return _gz; }

private:
    Uext& _uext;
    HMC5883L* _hmc;
    MPU6050*  _mpu;
    KALMAN    _pfilter;
    KALMAN    _rfilter;

    Vec3<float> _mag;    // uT (sensor axes)
    float _ax, _ay, _az; // m/s^2
    float _gx, _gy, _gz; // deg/s
    float _pitchDeg, _rollDeg;
    float _fpitch, _froll;
    float _pitchFilt, _rollFilt;   // low-passed tilt angles for heading
    Vec3<float> _magFilt;          // low-passed magnetometer for heading
    float _heading;
};
