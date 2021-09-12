#include "pid.h"

namespace lutil {

PIDController::PIDController(float kp, float ki, float kd, float target)
    : _kp(kp)
    , _ki(ki)
    , _kd(kd)
    , _target(target)
{
}


float PIDController::compute(float input, float delta)
{
    _p_error = _target - input;
    _i_error += _p_error * delta;
    _d_error = (_p_error - _previous_error) * delta;
    _previous_error = _p_error;

    // Tiny PID math
    float computed = (_kp * _p_error) + (_ki * _i_error) + (_kd * _d_error);
    computed = clamp(_min, computed, _max);
    return computed;
}


}