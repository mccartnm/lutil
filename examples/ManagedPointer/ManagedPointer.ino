#include "lutil.h" // Required
#include "lu_memory/managed_ptr.h"

namespace util = lutil::LUTIL_VERSION;

/*
    Structure that we'll use when experimenting below
*/
struct MyObject {
    const char *foo() { return "hello!"; }

    ~MyObject() {
        Serial.println("Destroyed!");
    }
};

/*
    Test function that adds a reference to our underlying
    pointer and then removes the reference on scope exit
*/
void test_foo(util::managed_ptr<MyObject> test) {

    // Confirm the underlying pointer holds a non-nullptr
    if (*test) {
        // Call the managed_ptr held object through the -> operator
        Serial.println(test->foo());
    }

}


void setup() {}

void loop() {

    // We'll create a scope to test the pointer
    {
        // Create an instance on the heap
        util::managed_ptr<MyObject> auto_clean(new MyObject());

        // Pass value to the function, creating a copy of the managed
        // pointer which adds a reference count.
        test_foo(auto_clean);

        // Afterwords, we are back down to a single reference
        // and, as we exit the scope, our destructor will run
        // on the object
        Serial.println("exiting scope...");
    }
    Serial.println("scope exited");

    delay(1000);
}
