#pragma once
#include "lutil.h"

namespace lutil {

/* Basic 3D vector */
struct LUTIL_API Axis
{
    float x;
    float y;
    float z;

    Axis();
    Axis(float a, float b, float c);

    Axis operator-(const Axis &other) const;
    Axis operator+(const Axis &other) const;
    Axis operator*(const Axis &other) const;
    Axis operator/(const Axis &other) const;

    Axis operator-(float scalar) const;
    Axis operator+(float scalar) const;
    Axis operator*(float scalar) const;
    Axis operator/(float scalar) const;

    float dot(const Axis &other);
};

}