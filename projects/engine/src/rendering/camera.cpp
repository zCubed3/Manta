#include "camera.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/quaternion.hpp>

namespace Manta {
    void Camera::UpdateMatrices() {
        glm::vec3 forward;

        if (lookAt)
            forward = glm::normalize(target - position);
        else
            forward = glm::quat(glm::radians(euler)) * glm::vec3(0, 0, 1);

        view = glm::lookAt(position, position + forward, glm::vec3(0, 1, 0));
        perspective = glm::perspective(glm::radians(fov), aspect, 0.001f, 100.0f);

        eye = perspective * view;
    }
}