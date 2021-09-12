/*
    Button logic for better interfaces
*/
#pragma once
#include "lutil.h"
#include "process.h"

#define DEBOUNCE_DELAY 50 // ms

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
template<size_t PIN>
class Button : public Processable {
public:

    Button()
        : Processable()
        , _active_state(ButtonState::Up)
        , _change_start(0)
        , _last_read(false)
    {}

    ButtonState state() const {
        return _active_state;
    }

    void init() override {
        pinMode(PIN, INPUT);
    }

    void process() override {
        auto read = digitalRead(PIN);

        //
        // Tiny debounce filter for any noise. Callbacks are
        // expensive so we need to make sure that they are
        // requested.
        //
        if (read != _last_read) {
            _change_start = millis();
        }

        if ((millis() - _change_start) > DEBOUNCE_DELAY) {
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
                    event(int(_active_state));
                }
                break;
            }
            case ButtonState::Up:
            {
                if (read == HIGH) {
                    _active_state = ButtonState::Pressed;
                    event(int(_active_state));
                }
                break;
            }
            }
        }

        _last_read = read;
    }


private:
    ButtonState _active_state;

    unsigned long _change_start;
    bool _last_read;
};

}
