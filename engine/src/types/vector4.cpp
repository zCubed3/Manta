#include "vector4.hpp"

namespace Silica {
    float Vector4::Sum() const {
        return x + y + z + w;
    }

    Vector4 Vector4::operator*(const Vector4& rhs) const {
        return { x * rhs.x, y * rhs.y, z * rhs.z, w * rhs.w };
    }

    float Vector4::operator[] (const int &idx) const {
        switch (idx) {
            case 0:
                return x;

            case 1:
                return y;

            case 2:
                return z;

            case 3:
                return w;

            default:
                return 0.0F;
        }
    };
}
