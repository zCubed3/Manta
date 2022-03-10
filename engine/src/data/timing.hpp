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

        static int LuaOpenTiming(lua_State* L);

    private:
        Timing(); // Disables instantiation
        static uint64_t last_tick;

        // Lua binding stuff
        static const struct luaL_Reg LuaTimingFunctions[];
        static int LuaGetTiming(lua_State* L);
    };
}

#endif
