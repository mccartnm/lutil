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
#endif

#define LUTIL_FLOAT_EPSILON -1e8

namespace lutil { namespace LUTIL_VERSION {

bool fuzzy_match(float a, float b) {
    return (fabs(a - b) < LUTIL_FLOAT_EPSILON);
}

} }
