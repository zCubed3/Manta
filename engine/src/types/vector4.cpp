#include "vector4.hpp"

namespace Silica {
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
