#ifndef SILICA_VECTOR4_HPP
#define SILICA_VECTOR4_HPP

namespace Silica {
    struct Vector4 {
        float x = 0.0F;
        float y = 0.0F;
        float z = 0.0F;
        float w = 0.0F;

        float operator[](const int &idx) const;
    };
}

#endif
