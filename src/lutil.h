/*
    Lightweight Util and Template Library for
    embedded systems.
*/
#pragma once
#define LUTIL_VERSION v_1
#define LUTIL_API

// Required for types
#ifndef BUILD_LIB
#ifndef CORE_TEENSY
# include "Arduino.h"
#else
# include "WProgram.h"
#endif
#else
// Non uC build
#include <cmath>
#include <cstring>
#include <stdint.h>
#ifdef WIN32
#undef LUTIL_API
#ifdef LUTIL_IMPORT
#define LUTIL_API __declspec(dllimport)
#else
#define LUTIL_API __declspec(dllexport)
#endif
#else
#define LUTIL_API
#endif
#endif

namespace lutil {}
