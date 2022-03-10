#ifndef SILICA_MATRIX4X4_HPP
#define SILICA_MATRIX4X4_HPP

#include "vector3.hpp"
#include "vector4.hpp"

namespace Silica {
    struct Matrix4x4 {
        Vector4 r0, r1, r2, r3;

        Vector4 Column0() const;
        Vector4 Column1() const;
        Vector4 Column2() const;
        Vector4 Column3() const;

        float* ValuePtr();

        static Matrix4x4 MakeRotationX(const float& degrees);
        static Matrix4x4 MakeRotationY(const float& degrees);
        static Matrix4x4 MakeRotationZ(const float& degrees);
        static Matrix4x4 MakeRotation(const Vector3& euler);

        static Matrix4x4 MakeTranslation(const Vector3& translation);
        static Matrix4x4 MakeScale(const Vector3& scale);

        static Matrix4x4 MakeTRS(const Vector3& translation, const Vector3& euler, const Vector3& scale);

        static Matrix4x4 MakePerspective(float fov_y, float aspect, float near_cull, float far_cull);

        Vector4& operator[] (const int &idx);
        Matrix4x4 operator* (const Matrix4x4& rhs) const;

        static Matrix4x4 Identity();
    };
}

#endif
