/*
    Example of a state machine using the lutil framework

    A lutil StateDriver is a spin on a traditional state machine and
    has two main types of functionality.

    Transitions:
        - The "test" code run to see if the machine is ready to swtch
        from one state to another. This is useful when one state doesn't
        know what other states exist

    Runtimes:
        - Code that's run while in a given state. This can also alter the
        state of the machine to change which runtimes/transitions are
        active.

    If this is a bit confusing, consider the following example. We have
    a digital switch that will adjust it's state whenever a push button
    is pressed. There is a runtime for the On state to fade the Led in
    and out.
*/
#include "lutil.h"
#include "lu_state/state.h"
#include "lu_process/button.h"

using namespace lutil::v_1;

Button<5> go_button;  // Basic push button on pin 5
Led on_light(9);      // Basic single color LED on pin 9

/*
    Example states available. These can be any string you'd like and
    there is no state size limit.
*/
constexpr char Off[] = "Off";
constexpr char On[] = "On";

class DigitalSwitch : public StateDriver<DigitalSwitch>
{
public:
    explicit DigitalSwitch()
        : StateDriver<DigitalSwitch>()
        , _on(false)
    {
        //
        // Define our state machine transitions
        //

        // Off -> On : An odd button press
        add_transition(Off, On, &DigitalSwitch::check_on);

        // While in the On state...
        add_runtime(On, &DigitalSwitch::while_on);

        // On -> Off : An even button press
        add_transition(On, Off, &DigitalSwitch::check_off);
    }

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
        if (!_on) {
            Serial.println("SET OFF");
            on_light.set_value(0); // Disable the light
        }
        return !_on;
    }

    // RUNTIME FUNCTIONS

    void while_on() { on_light.fade(1000); }

private:
    bool _on;

    // I would give my first born for reflection to be in the
    // standard already... one day...
    BEGIN_DRIVER_DEFINITION
        TRANSITION(DigitalSwitch, check_on);
        TRANSITION(DigitalSwitch, check_off);
        RUNTIME(DigitalSwitch, while_on);
    END_DRIVER_DEFINITION
};

// Create out digital switch instance
DigitalSwitch sw;

void setup() {
    // Combine the button process with the state machine
    // via the lutil Processor. This allows for a clean event
    // loop and avoids additional coupling between the button
    // and the state machine.
    Processor::get().init();
}


void loop() {
    //
    // This is it! We don't need anything extra. The processor
    // will do all the work
    //
    // The Button and DigitalSwitch are Processables which automatically
    // register themselves with the global Processor instance when they
    // are constructed. This process loop will handle the events that
    // we want to work with without having to overload the .ino with
    // boilerplate
    //
    Processor::get().process();
    delay(5); // Not required. Just lighten the sim a little
}
