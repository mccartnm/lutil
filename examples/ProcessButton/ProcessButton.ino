/*
    Simple example of using the Processor to understand different events
    from general input items (button, switch, etc).

    To run, hook up a push button to any pin and adjust the PIN below
    (this example defaults to pin 5).
*/
#include "lutil.h" // Required
#include "lu_process/process.h"
#include "lu_process/button.h"


namespace util = lutil::LUTIL_VERSION;

/*
    Change the PIN to match whatever button setup you're using.
*/
util::Button<5> button;


void button_pressed() {
    Serial.println("__PRESS__");
}

void button_released() {
    Serial.println("__RELEASE__");
}

void setup() {
    Serial.begin(115200);

    util::Processor &proc = util::Processor::get();
    proc.add_processable(&button);

    // Add callbacks
    button.when((int)util::ButtonState::Pressed, button_pressed);
    button.when((int)util::ButtonState::Released, button_released);

    proc.init(); // Initialize all processables
}


void loop() {
    util::Processor::get().process(); // Check for updates
    delay(5); // A little breathing room
}