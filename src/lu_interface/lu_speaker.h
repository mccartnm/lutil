#pragma once
#include "lutil.h"

namespace lutil
{

class Buzzer
{
public:
    Buzzer(int pin)
        : _pin(pin)
    {
        pinMode(_pin, OUTPUT);
    }

    void beep(uint32_t milliseconds)
    {
        digitalWrite(_pin, HIGH);
        delay(milliseconds);
        digitalWrite(_pin, LOW);        
    }

private:
    int _pin;
};

}