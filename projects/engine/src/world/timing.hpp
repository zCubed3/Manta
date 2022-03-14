#ifndef MANTA_TIMING_HPP
#define MANTA_TIMING_HPP

#include <cstdint>
#include <glm/vec4.hpp>

namespace Manta {
    class Timing final {
    public:
        static float delta_time;
        static float time;

        // X = 1, Y = 2, Z = 4, W = 8;
        static glm::vec4 sin_time, cos_time, tan_time;

        static void UpdateTime();

    private:
        Timing(); // Disables instantiation
        static uint64_t last_tick;
    };
}

#endif
