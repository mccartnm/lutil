#pragma once
#include "lutil.h"

#ifdef CORE_TEENSY
#include <PWMServo.h>
#define Servo PWMServo
#else
#include <Servo.h>
#endif

namespace lutil
{

/**
 * Basic servo wrapper. Useful for retaining information about the
 * state of a servo at any given time.
 */
class lu_Servo
{
public:
    lu_Servo(int pin = -1, int min = 0, int max = 180);

    /**
     * Offset alignment of a servo. Useful for handling real
     * world problem of physical alignment.
     */
    int alignment() const { return _align; }
    void set_alignment(int align);

    int minimum() const { return _min; }
    void set_min(int min);

    int maximum() const { return _max; }
    void set_max(int max);

    int current() const { return _current; }

    void init();
    void write(int degrees);

    // Status test(float start_time); ?

private:
    int _pin;
    Servo _servo;
    int _current;

    int _min;
    int _max;
    int _align;
};

}