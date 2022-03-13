#ifndef SILICA_CAMERA_HPP
#define SILICA_CAMERA_HPP

// TODO: Phase out Silica math since it's bad
//#include <types/vector3.hpp>
//#include <types/matrix4x4.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

namespace Manta {
    class Camera {
    public:
        glm::vec3 position = glm::vec3(0, 0, 1);
        glm::vec3 euler = glm::vec3(0, 0, 0);
        float fov = 60;
        float aspect = 1.0;

        // TODO: Make a better alternative eventually!
        bool lookAt = false;
        glm::vec3 target = glm::vec3(0, 0, 0);

        glm::mat4x4 view, perspective, eye;

        void UpdateMatrices();
    };
}

#endif
