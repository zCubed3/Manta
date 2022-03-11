#ifndef SILICA_TIMING_HPP
#define SILICA_TIMING_HPP

#include <cstdint>
#include <lua_common.hpp>

namespace Silica {
    class Timing final {
    public:
        static float delta_time;
        static float time;

        static void UpdateTime();

    private:
        Timing(); // Disables instantiation
        static uint64_t last_tick;
    };
}

#endif
