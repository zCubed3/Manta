#ifndef MANTA_VIEWPORT_HPP
#define MANTA_VIEWPORT_HPP

#include <world/transform.hpp>

#include <vector>

namespace Manta {
    class Viewport {
    public:
        static std::vector<Viewport*> active_viewports;
        static Viewport* active_viewport;

        uint16_t width = 512, height = 512;
        uint16_t x = 0, y = 0;
        float fov = 60;
        glm::vec3 clear_color = glm::vec3(0.1f, 0.1f, 0.1f);
        bool clear = true, clear_depth = true;

        Transform transform;

        glm::mat4x4 perspective, eye;

        Viewport();
        void UpdateMatrices();
    };
}

#endif
