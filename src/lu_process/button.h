/*
    Button logic for better interfaces
*/
#pragma once
#include "lutil.h"
#include "process.h"

namespace lutil {

enum class ButtonState {
    Pressed,   // CALLBACK CAPABLE
    Released,  // CALLBACK CAPABLE
    Down,
    Up
};

/*
    Implements a basic debouncing as well as allows event consumption
    from a higher level loop (a-la the main program loop)
*/
class Button : public Processable {
public:

    Button(int pin, int debounce = 50)
        : Processable()
        , _active_state(ButtonState::Up)
        , _pin(pin)
        , _debounce(debounce)
        , _change_start(0)
        , _last_read(false)
    {}

    ButtonState state() const
    {
        return _active_state;
    }

    void init() override
    {
        pinMode(_pin, INPUT);
    }

    void process() override
    {
        auto read = digitalRead(_pin);

        //
        // Tiny debounce filter for any noise. Callbacks are
        // expensive so we need to make sure that they are
        // requested.
        //
        if (read != _last_read) {
            _change_start = millis();
        }

        if ((millis() - _change_start) > _debounce) {
            // State vessle for button reading
            switch(_active_state)
            {
            case ButtonState::Pressed:
            {
                if (read == LOW) {
                    // User let go in between
                    _active_state = ButtonState::Released;
                }
                else {
                    _active_state = ButtonState::Down;
                }
                break;
            }
            case ButtonState::Released:
            {
                if (read == HIGH) {
                    // User pressed in between?
                    _active_state = ButtonState::Pressed;
                }
                else {
                    _active_state = ButtonState::Up;
                }
                break;
            }
            case ButtonState::Down:
            {
                // We've let go of the button
                if (read == LOW) {
                    _active_state = ButtonState::Released;
                    released();
                    event(int(_active_state));
                }
                break;
            }
            case ButtonState::Up:
            {
                if (read == HIGH) {
                    _active_state = ButtonState::Pressed;
                    pressed();
                    event(int(_active_state));
                }
                break;
            }
            }
        }

        _last_read = read;
    }

    void set_debounce(int debounce)
    {
        _debounce = debounce;
    }

protected:

    // Overloadable for fun and profit
    virtual void released() {}
    virtual void pressed() {}

private:
    ButtonState _active_state;

    int _pin;
    uint8_t _debounce;
    unsigned long _change_start;
    bool _last_read;
};

}
