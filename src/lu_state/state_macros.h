/*
    A means of dynamic state machine control
*/
#pragma once

#define BEGIN_DRIVER_DEFINITION  \
protected:                       \
    void _register_machine() override {

#define RUNTIME(mname, name) \
    _register_runtime(#name, &mname::name)

#define TRANSITION(mname, name) \
    _register_transition(#name, &mname::name)

#define END_DRIVER_DEFINITION }