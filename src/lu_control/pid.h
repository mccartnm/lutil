/*
    Proportional, Integral, Derivative mechanism for computing
    output values more effectively in a small-scale system
*/
#include "lutil.h"
#include "lu_math/matrix.h"

namespace lutil {

class LUTIL_API PIDController {

public:
    PIDController(float kp = 1.00, float ki = 0, float kd = 0, float _target = 1.0);

    /* Main computation function - run per-iteration */
    float compute(float input, float delta);

    float Kp() const { return _kp; }
    float Ki() const { return _ki; }
    float Kd() const { return _kd; }
    float target() const { return _target; }

    void set_Kp(float kp) { _kp = kp; }
    void set_Ki(float ki) { _ki = ki; }
    void set_Kd(float kd) { _kd = kd; }
    void set_target(float target) { _target = target; } // aka: SP

    float error() const { return _p_error; }
    float integral_error() const { return _i_error; }
    float derivative_error() const { return _d_error; }
    float previous_error() const { return _previous_error; }

    // Limits
    float maximum() const { return _max; }
    void set_max(float maximum) { _max = maximum; }

    float minimum() const { return _min; }
    void set_min(float minimum) { _min = minimum; }

private:
    float _kp; // Proportional gain
    float _ki; // Integral gain
    float _kd; // Derivative gain

    // The value that we want the control to work towards
    float _target;

    /*
        Error fundamentals
    */
    float _p_error;
    float _i_error;
    float _d_error;
    float _previous_error;

    /*
        Constraints
    */
    float _min = 0.0;
    float _max = 1.0;
};

}
