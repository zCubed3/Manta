#ifndef MANTA_LIGHTBUFFER_HPP
#define MANTA_LIGHTBUFFER_HPP

#include <cstdint>

#include <glm/vec4.hpp>

namespace Manta {
    struct LightData {
        glm::vec4 color_w_intensity;
        glm::vec4 position_w_type;
    };

    class LightBuffer {
    public:
        #define MAX_LIGHT_COUNT 24
        const uint32_t BUFFER_SIZE = sizeof(LightData) * MAX_LIGHT_COUNT + sizeof(uint32_t);

        LightData data[MAX_LIGHT_COUNT];
        uint32_t light_count;

        void CreateBuffer();
        void UpdateBuffer();

        uint32_t handle;
    };
}

#endif
