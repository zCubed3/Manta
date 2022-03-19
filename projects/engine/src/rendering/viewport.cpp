#include "viewport.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/quaternion.hpp>

namespace Manta {
    void Viewport::UpdateMatrices() {
        float aspect = (float)width / (float)height;
        perspective = glm::perspective(glm::radians(fov), aspect, z_near, z_far);

        eye = perspective * view;
    }
}