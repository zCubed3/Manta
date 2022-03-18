#include "viewport.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/quaternion.hpp>

namespace Manta {
    Viewport::Viewport() {

    }

    void Viewport::UpdateMatrices() {
        float aspect = (float)width / (float)height;
        perspective = glm::perspective(glm::radians(fov), aspect, 0.001f, 100.0f);

        transform.UpdateMatrices();

        eye = perspective * transform.world_to_local;
    }
}