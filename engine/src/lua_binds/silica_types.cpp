#include "silica_types.hpp"

namespace Silica::LuaBindings {
    int lua_open_silica(lua_State* L) {
        luaL_requiref(L, "timing", LuaBindings::lua_open_timing, 1);
        luaL_requiref(L, "vector2", LuaBindings::lua_open_vector2, 1);
        luaL_requiref(L, "vector3", LuaBindings::lua_open_vector3, 1);
        luaL_requiref(L, "actor", LuaBindings::lua_open_actor, 1);
        luaL_requiref(L, "world", LuaBindings::lua_open_world, 1);
        luaL_requiref(L, "behavior", LuaBindings::lua_open_behavior, 1);
    }
}