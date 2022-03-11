#ifndef SILICA_LUA_BINDS_ACTOR_HPP
#define SILICA_LUA_BINDS_ACTOR_HPP

#include <lua_common.hpp>

namespace Silica {
    class Actor;
}

namespace Silica::LuaBindings {
    struct LuaActorReference {
        Actor* actor;
    };

    int lua_open_actor(lua_State *L);
}

#endif
