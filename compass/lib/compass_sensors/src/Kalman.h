#pragma once
// 1-axis Kalman filter for fusing an accelerometer angle with a gyroscope
// rate. Ported verbatim (state variables renamed) from the akkaEsp32
// DigitalCompass component.

class KALMAN {
public:
    explicit KALMAN(float dt);
    float filter(float accel, float gyro);
    void setDt(float dt) { _dt = dt; }

private:
    float _angle = 0.0f;
    float _qBias = 0.0f;
    float _angleErr = 0.0f;

    float _qAngle = 0.1f;
    float _qGyro  = 0.1f;
    float _rAngle = 0.5f;
    float _dt;

    float _c0 = 1.0f;
    float _pct0 = 0.0f, _pct1 = 0.0f, _e = 0.0f;
    float _k0 = 0.0f, _k1 = 0.0f, _t0 = 0.0f, _t1 = 0.0f;
    float _pdot[4] = {0, 0, 0, 0};
    float _pp[2][2] = {{1, 0}, {0, 1}};
};
