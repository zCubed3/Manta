#include "timing.hpp"

#include <iostream>

#include <world/timing.hpp>

namespace Silica::LuaBindings {
    int lua_time_timing(lua_State* L) {
        lua_pushnumber(L, Timing::time);
        return 1;
    }

    int lua_delta_time_timing(lua_State* L) {
        lua_pushnumber(L, Timing::delta_time);
        return 1;
    }

    const struct luaL_Reg lua_timing_functions[] = {
            {"time", lua_time_timing},
            {"delta_time", lua_delta_time_timing},
            {nullptr, nullptr}
    };

    int lua_open_timing(lua_State* L) {
        luaL_newlib(L, lua_timing_functions);
        return 1;
    }
}