/*
    State management toolkit
*/
#pragma once
#include "lutil.h"
#include "lu_storage/map.h"

namespace lutil { namespace LUTIL_VERSION {


class _AbstractDriver {
public:
    _AbstractDriver() {}
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
template<class Derived, typename St, size_t Initial = 0>
class StateDriver : public _AbstractDriver {
public:
    typedef bool (Derived::*TransitionPredicate)();
    using PredicateMap = Map<St, TransitionPredicate>;

    StateDriver() { _current_state = initial_state(); }

    St current_state() const { return _current_state; }

    bool add_transition(
        St from_state,
        St to_state,
        TransitionPredicate predicate)
    {
        if (!_predicates.contains(from_state)) {
            _predicates[from_state] = PredicateMap();
        }

        PredicateMap &map = _predicates[from_state];
        if (map.contains(to_state))
            return false;

        map[to_state] = predicate;
        return true;
    }

    void process() {
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

protected:
    St initial_state() const { return St(Initial); };

private:
    St _current_state;
    Map<St, PredicateMap> _predicates;
};

}
}
