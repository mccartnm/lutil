#include "axis.h"

namespace lutil
{

Axis::Axis()
    : x(0)
    , y(0)
    , z(0)
{}

Axis::Axis(float a, float b, float c)
    : x(a)
    , y(b)
    , z(c)
{}

#define VECTOR_OP(op)                                        \
    Axis Axis::operator op(const Axis &other) const {        \
        return { x op other.x, y op other.y, z op other.z }; \
    }

VECTOR_OP(-)
VECTOR_OP(+)
VECTOR_OP(*)
VECTOR_OP(/)

#define SCALAR_OP(op)                                     \
    Axis Axis::operator op(float scalar) const {          \
        return { x op scalar, y op scalar, z op scalar }; \
    }

SCALAR_OP(-)
SCALAR_OP(+)
SCALAR_OP(*)
SCALAR_OP(/)


float Axis::dot(const Axis &other)
{
    return (x * other.x) + (y * other.y) + (z * other.z);
}


}


