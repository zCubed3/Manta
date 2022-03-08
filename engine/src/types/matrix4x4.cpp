#include "matrix4x4.hpp"

namespace Silica {
    Vector4 Matrix4x4::operator[] (const int &idx) const {
        switch (idx) {
            case 0:
                return r0;

            case 1:
                return r1;

            case 2:
                return r2;

            case 3:
                return r3;

            default:
                return {};
        }
    }

    Matrix4x4 Matrix4x4::identity() {
        return {
            { 1, 0, 0, 0 },
            { 0, 1, 0, 0 },
            { 0 ,0, 1, 0 },
            { 0, 0, 0, 1 }
        };
    };
}