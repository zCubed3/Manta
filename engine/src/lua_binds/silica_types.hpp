#ifndef SILICA_LUA_BINDS_TYPES_HPP
#define SILICA_LUA_BINDS_TYPES_HPP

#include "vec2.hpp"
#include "vec3.hpp"

#include "world.hpp"
#include "actor.hpp"
#include "timing.hpp"
#include "behavior.hpp"

#include "lua_common.hpp"

namespace Silica::LuaBindings {
    int lua_open_silica(lua_State* L);
}

#endif
