#ifndef SILICA_VECTOR4_HPP
#define SILICA_VECTOR4_HPP

namespace Silica {
    struct Vector4 {
        float x = 0.0F;
        float y = 0.0F;
        float z = 0.0F;
        float w = 0.0F;

        Vector4(float x = 0, float y = 0, float z = 0, float w = 0) {
            this->x = x;
            this->y = y;
            this->z = z;
            this->w = w;
        }

        float Sum() const;

        Vector4 operator*(const Vector4& rhs) const;
        float operator[](const int &idx) const;
    };
}

#endif
