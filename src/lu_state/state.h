/*
    State management toolkit
*/
#pragma once
#include "lutil.h"
#include "lu_storage/map.h"
#include "lu_process/process.h"
#include "lu_memory/managed_ptr.h"

namespace lutil { namespace LUTIL_VERSION {

/*
    Abstract class to help with passing a state machine about.

    Without this, the template class below becomes more challenging to
    build into more complex systems (plug-n-play, etc.)
*/
class _AbstractDriver : public Processable {
public:
    _AbstractDriver() : Processable() {}

    void init() override {}

protected:
    virtual const char *initial_state() const { return "Off"; }
};


/*
    This is a CRTP to keep higher-level code to a minimum
    - https://en.wikipedia.org/wiki/Curiously_recurring_template_pattern

    A well defined state machine template utility. Scales larger than
    the typical switch statement utility and allows for explicit definition
    of flow outside of the class definition. This means we can handle the
    state transitions in an abstract way and "program" it more toward
    runtime.
*/
template<class Derived>
class StateDriver : public _AbstractDriver {
public:
    typedef bool (Derived::*TransitionPredicate)();
    typedef void (Derived::*RuntimeFunction)();
    using PredicateMap = Map<uint16_t, TransitionPredicate>;

    StateDriver() {
        _current_state = _initial_state();
    }

    managed_string current_state() const {
        return _known_states.key_from_value(_current_state);
    }

    bool add_runtime(managed_string state, RuntimeFunction func) {
        uint16_t state_id = _state_id(state);
        if (!_runtimes.contains(state_id)) {
            _runtimes[state_id] = {};
        }
        _runtimes[state_id].push(func);
        return true;
    }

    bool add_transition(
        managed_string from_state,
        managed_string to_state,
        TransitionPredicate predicate)
    {
        uint16_t from_id = _state_id(from_state);
        uint16_t to_id = _state_id(to_state);

        if (!_predicates.contains(from_id)) {
            _predicates[from_id] = PredicateMap();
        }

        PredicateMap &map = _predicates[from_id];
        if (map.contains(to_id))
            return false;

        map[to_id] = predicate;
        return true;
    }

    void process() override {
        if (_runtimes.contains(_current_state)) {
            Vec<RuntimeFunction> &runtimes = _runtimes[_current_state];
            auto it = runtimes.begin();
            uint16_t original_state = _current_state;
            for (; it != runtimes.end(); it++) {
                Derived *self = static_cast<Derived *>(this);
                RuntimeFunction &func = *it;
                (self->*func)();
                if (_current_state != original_state) {
                    // last runtime has changed the state
                    return;
                }
            }
        }

        if (_predicates.contains(_current_state)) {
            PredicateMap &map = _predicates[_current_state];
            auto it = map.begin();

            for (; it != map.end(); it++) {
                TransitionPredicate &predicate = it.value();
                Derived *self = static_cast<Derived *>(this);
                if ((self->*predicate)()) {
                    _current_state = it.key();
                    return;
                }
            }

        }
    }

private:
    uint16_t _state_id(managed_string state) {
        if (!_known_states.contains(state)) {
            _known_states[state] = (uint16_t)_known_states.count();
        }
        return _known_states[state];
    }

    /*
        We have to make sure that initial state has an id
    */
    uint16_t _initial_state() {
        managed_string st = initial_state();
        return _state_id(st);
    }

    uint16_t _current_state;

    /*
        We have three mappings

        - each known state recieves a unique ID at runtime.
        - Each state can contain transition predicates that will
          automatically switch to another state upon some requirement
          being true
        - Each state can contain a number of runtime proceedures
    */
    Map<managed_string, uint16_t> _known_states;
    Map<uint16_t, PredicateMap> _predicates;
    Map<uint16_t, Vec<RuntimeFunction>> _runtimes;
};

}
}
