/*
    Example of a state machine using the lutil framework
*/
#include "lutil.h"
#include "lu_state/state.h"
#include "lu_process/button.h"

using namespace lutil::v_1;

Button<5> go_button;  // Basic push button on pin 5
Led on_light(9);      // Basic single color LED on pin 9

/*
    Example states available
*/
constexpr char Off[] = "Off";
constexpr char On[] = "On";

class DigitalSwitch : public StateDriver<DigitalSwitch>
{
public:
    explicit DigitalSwitch()
        : StateDriver<DigitalSwitch>()
        , _on(false)
    {}

    // TRANSITION FUNCTIONS

    bool check_on() {
        // Only called when in the Off state
        _on = (go_button.state() == ButtonState::Pressed);
        if (_on)
            Serial.println("SET ON");
        return _on;
    }

    bool check_off() {
        // Only called when in the On state
        _on = !(go_button.state() == ButtonState::Pressed);
        if (!_on)
            Serial.println("SET OFF");
        return !_on;
    }


    // RUNTIME FUNCTIONS

    void while_on() { on_light.fade(1000); }

    void while_off() { on_light.set_value(0); }


private:
    bool _on;
};

// Create out digital switch instance
DigitalSwitch eng;

void setup() {
    // Define our state machine transitions

    // Off -> On : An odd button press
    eng.add_transition(
        Off, On,
        &DigitalSwitch::check_on
    );
    eng.add_runtime(On, &DigitalSwitch::while_on);

    // On -> Off : An even button press
    eng.add_transition(
        On, Off,
        &DigitalSwitch::check_off
    );
    eng.add_runtime(Off, &DigitalSwitch::while_off);

    // Combine the button process with the state machine
    // via the lutil Processor. This allows for a clean event
    // loop and avoids additional coupling between the button
    // and the state machine.
    Processor::get().init();
}


void loop() {
    // This is it! We don't need anything extra. The processor
    // will do all the work
    Processor::get().process();
    delay(5); // Not required. Just lighten the sim a little
}
