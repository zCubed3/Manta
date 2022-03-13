#include "transform.hpp"

#include <glm/gtx/transform.hpp>
#include <glm/gtx/quaternion.hpp>

namespace Manta {
    Transform::Transform(glm::vec3 position, glm::vec3 euler, glm::vec3 scale) {
        this->position = position;
        this->euler = euler;
        this->scale = scale;
    }

    void Transform::UpdateMatrices() {
        local_to_world = glm::translate(glm::mat4(1.0f), position);
        local_to_world *= glm::toMat4(glm::quat(glm::radians(euler)));
        local_to_world = glm::scale(local_to_world, scale);

        world_to_local = glm::inverse(local_to_world);
        world_to_local_t = glm::transpose(world_to_local);
    }
}