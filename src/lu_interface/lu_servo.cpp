#include "lu_servo.h"
#include "lu_math/matrix.h"

namespace lutil
{

lu_Servo::lu_Servo(int pin, int min, int max)
    : _pin(pin)
    , _current(-1)
    , _min(min)
    , _max(max)
    , _align(0)
{
}

void lu_Servo::set_alignment(int align)
{
    _align = align;
    write(_current);
}

void lu_Servo::set_min(int min)
{
    _min = min;
    write(_current);
}

void lu_Servo::set_max(int max)
{
    _max = max;
    write(_current);
}

void lu_Servo::init()
{
    if (_pin < 0)
        return;

    _servo.attach(_pin);
    _current = -1;

    // -- ?
    write((_min + _max) / 2);
}

void lu_Servo::write(int degrees)
{
    if (degrees < 0)
        return;

    degrees = clamp(_min, degrees, _max);
    _servo.write(degrees + _align);
    _current = degrees;
}


}
