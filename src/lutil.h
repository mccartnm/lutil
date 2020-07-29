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

namespace lutil { namespace LUTIL_VERSION {} }
