#include "viewport.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/quaternion.hpp>

#include "render_target.hpp"

namespace Manta::Rendering {
    void Viewport::Update() {
        if (render_target) {
            width = render_target->width;
            height = render_target->height;
        }

        float aspect = (float)width / (float)height;
        perspective = glm::perspective(glm::radians(fov), aspect, z_near, z_far);

        eye = perspective * view;
    }
}