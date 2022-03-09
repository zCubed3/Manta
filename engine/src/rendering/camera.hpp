#ifndef SILICA_CAMERA_HPP
#define SILICA_CAMERA_HPP

#include <types/vector3.hpp>
#include <types/matrix4x4.hpp>

namespace Silica {
    class Camera {
    public:
        Vector3 position = Vector3(0, 0, -1), rotation;
        float fov = 90;

        Matrix4x4 view, perspective;

        void UpdateMatrices();
    };
}

#endif
