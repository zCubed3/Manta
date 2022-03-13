#include "viewport.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/quaternion.hpp>

namespace Manta {
    std::vector<Viewport*> Viewport::active_viewports = {};
    Viewport* Viewport::active_viewport = nullptr;

    Viewport::Viewport() {
        transform.only_local = true;
    }

    void Viewport::UpdateMatrices() {
        float aspect = (float)width / (float)height;
        perspective = glm::perspective(glm::radians(fov), aspect, 0.001f, 100.0f);

        transform.UpdateMatrices();

        eye = perspective * glm::inverse(transform.local_to_world);
    }
}