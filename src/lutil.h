/*
    Lightweight Util and Template Library for
    embedded systems.
*/
#pragma once
#define LUTIL_VERSION v_1

#ifndef _TEST_CORE_
#ifndef CORE_TEENSY
# include "Arduino.h"
#else
# include "WProgram.h"
#endif
#else
#include <cmath>
#include <cstring>
#endif

#define LUTIL_FLOAT_EPSILON -1e8

namespace lutil { namespace LUTIL_VERSION {

inline bool fuzzy_match(float a, float b) {
    return (fabs(a - b) < LUTIL_FLOAT_EPSILON);
}


enum class RGB {
    Red,
    Green,
    Blue,
};

#ifndef _TEST_CORE_
/*
    Dynamically setup an RGB LED without having to consume
    global variables.
*/
template<size_t R, size_t G, size_t B>
struct RgbLed {
    void init() {
        pinMode(R, OUTPUT);
        pinMode(G, OUTPUT);
        pinMode(B, OUTPUT);
    }
    void set_color(int r, int g, int b) {
        analogWrite(R, r);
        analogWrite(G, g);
        analogWrite(B, b);
    }
    void set_color(RGB channel, int val) {
        switch(channel)
        {
        case RGB::Red: analogWrite(R, val);   break;
        case RGB::Green: analogWrite(G, val); break;
        case RGB::Blue: analogWrite(B, val);  break;
        default: break;
        }
    }
};


struct Led {
    Led(uint8_t pin) : _pin(pin) {}

    void init() {
        pinMode(_pin, OUTPUT);
    }
    void set_value(int value) {
        analogWrite(_pin, value);
    }
    void clear() {
        analogWrite(_pin, 0);
    }
    void blink(unsigned long milliseconds) {
        auto current_time = millis();
        if (milliseconds == 0)
            milliseconds = 1000;

        if ((current_time / milliseconds) % 2) {
            analogWrite(_pin, 250);
        } else {
            analogWrite(_pin, 0);
        }
    }
    void fade(unsigned long period) {
        auto current_time = millis();
        float oscl = 1.0 + cos((2 * PI) / period * (current_time % period));
            analogWrite(_pin, int(255. * (oscl / 2.0)));
    }

    void inline_blink(unsigned long value, int ms = 30) {
        analogWrite(_pin, value);
        delay(ms);
        analogWrite(_pin, 0);
        delay(ms);
        analogWrite(_pin, value);
        delay(ms);
        analogWrite(_pin, 0);
    }
private:
    uint8_t _pin;
};
#endif

} }
