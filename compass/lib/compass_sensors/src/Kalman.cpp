#include "Kalman.h"

KALMAN::KALMAN(float dt) : _dt(dt) {}

float KALMAN::filter(float accel, float gyro)
{
    _angle += (gyro - _qBias) * _dt;
    _angleErr = accel - _angle;

    _pdot[0] = _qAngle - _pp[0][1] - _pp[1][0];
    _pdot[1] = -_pp[1][1];
    _pdot[2] = -_pp[1][1];
    _pdot[3] = _qGyro;
    _pp[0][0] += _pdot[0] * _dt;
    _pp[0][1] += _pdot[1] * _dt;
    _pp[1][0] += _pdot[2] * _dt;
    _pp[1][1] += _pdot[3] * _dt;

    _pct0 = _c0 * _pp[0][0];
    _pct1 = _c0 * _pp[1][0];

    _e = _rAngle + _c0 * _pct0;

    _k0 = _pct0 / _e;
    _k1 = _pct1 / _e;

    _t0 = _pct0;
    _t1 = _c0 * _pp[0][1];

    _pp[0][0] -= _k0 * _t0;
    _pp[0][1] -= _k0 * _t1;
    _pp[1][0] -= _k1 * _t0;
    _pp[1][1] -= _k1 * _t1;

    _angle += _k0 * _angleErr;
    _qBias += _k1 * _angleErr;

    return _angle;
}
