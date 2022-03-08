#include "matrix4x4.hpp"

#include <math.h>

#include <constants.hpp>

namespace Silica {
    Vector4 Matrix4x4::Column0() const {
        return { r0.x, r1.x, r2.x, r3.x };
    }

    Vector4 Matrix4x4::Column1() const {
        return { r0.y, r1.y, r2.y, r3.y };
    }

    Vector4 Matrix4x4::Column2() const {
        return { r0.z, r1.z, r2.z, r3.z };
    }

    Vector4 Matrix4x4::Column3() const {
        return { r0.w, r1.w, r2.w, r3.w };
    }

    Matrix4x4 Matrix4x4::MakeRotationX(const float& degrees) {
        auto mat = Matrix4x4::Identity();
        float rad = degrees * DEG_TO_RAD;

        mat[1] = Vector4(0, cosf(rad), -sinf(rad), 0);
        mat[2] = Vector4(0, sinf(rad), cosf(rad), 0);

        return mat;
    }

    Matrix4x4 Matrix4x4::MakeRotationY(const float& degrees) {
        auto mat = Matrix4x4::Identity();
        float rad = degrees * DEG_TO_RAD;

        mat[0] = Vector4(cosf(rad), 0, sinf(rad), 0);
        mat[2] = Vector4(-sinf(rad), 0, cosf(rad), 0);

        return mat;
    }

    Matrix4x4 Matrix4x4::MakeRotationZ(const float& degrees) {
        auto mat = Matrix4x4::Identity();
        float rad = degrees * DEG_TO_RAD;

        mat[0] = Vector4(cosf(rad), 0, -sinf(rad), 0);
        mat[1] = Vector4(sinf(rad), 0, cosf(rad), 0);

        return mat;
    }

    Matrix4x4 Matrix4x4::MakeRotation(const Vector3& euler) {
        return MakeRotationX(euler.x) * MakeRotationY(euler.y) * MakeRotationZ(euler.z);
    }

    Matrix4x4 Matrix4x4::MakeTranslation(const Vector3& translation) {
        auto mat = Matrix4x4::Identity();
        mat[3] = Vector4(translation.x, translation.y, translation.z, 1.0F);
        return mat;
    }

    Matrix4x4 Matrix4x4::MakeScale(const Vector3& scale) {
        auto mat = Matrix4x4::Identity();

        mat[0] = Vector4(scale.x, 0, 0, 0);
        mat[1] = Vector4(0, scale.y, 0, 0);
        mat[2] = Vector4(0, 0, scale.z, 0);

        return mat;
    }

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

    Matrix4x4 Matrix4x4::operator*(const Matrix4x4 &rhs) const {
        auto c0 = Column0();
        auto c1 = Column1();
        auto c2 = Column2();
        auto c3 = Column3();

        auto mat = Matrix4x4();

        mat.r0 = { (r0 * c0).Sum(), (r0 * c1).Sum(), (r0 * c2).Sum(), (r0 * c3).Sum() };
        mat.r1 = { (r1 * c0).Sum(), (r1 * c1).Sum(), (r1 * c2).Sum(), (r1 * c3).Sum() };
        mat.r2 = { (r2 * c0).Sum(), (r2 * c1).Sum(), (r2 * c2).Sum(), (r2 * c3).Sum() };
        mat.r3 = { (r3 * c0).Sum(), (r3 * c1).Sum(), (r3 * c2).Sum(), (r3 * c3).Sum() };

        return mat;
    };

    //
    // Defaults
    //
    Matrix4x4 Matrix4x4::Identity() {
        return {
            { 1, 0, 0, 0 },
            { 0, 1, 0, 0 },
            { 0 ,0, 1, 0 },
            { 0, 0, 0, 1 }
        };
    }
}