#ifndef SILICA_MATRIX4X4_HPP
#define SILICA_MATRIX4X4_HPP

#include "vector4.hpp"

namespace Silica {
    struct Matrix4x4 {
        Vector4 r0, r1, r2, r3;

        Vector4 operator[] (const int &idx) const;

        static Matrix4x4 identity();
    };
}

#endif
