#ifndef SILICA_CAMERA_HPP
#define SILICA_CAMERA_HPP

#include <types/vector3.hpp>
#include <types/matrix4x4.hpp>

namespace Silica {
    class Camera {
    public:
        Vector3 position = Vector3(0, 0, -1), rotation = Vector3(0, 180, 0);
        float fov = 60;
        float aspect = 1.0;

        // TODO: Make a better alternative eventually!
        bool lookAt = false;
        Vector3 target = Vector3();

        Matrix4x4 view, perspective, eye;

        void UpdateMatrices();
    };
}

#endif
