#ifndef MANTA_TIMING_HPP
#define MANTA_TIMING_HPP

#include <cstdint>
#include <glm/vec4.hpp>

namespace Manta {
    class Timing final {
    public:
        float delta_time = 0;
        float time = 0;

        // X = 1, Y = 2, Z = 4, W = 8;
        glm::vec4 sin_time, cos_time, tan_time;

        void UpdateTime();

    private:
        uint64_t last_tick;
    };
}

#endif
