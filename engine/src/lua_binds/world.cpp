#include "world.hpp"

#include <iostream>

#include <world/actor.hpp>
#include <world/world.hpp>

#include "actor.hpp"

#include "lua_helpers.hpp"

namespace Silica::LuaBindings {
    int lua_world_get_actor(lua_State* L) {
        std::string name = luaL_checkstring(L, -1);
        Actor* actor = World::FindActor(name);

        if (actor == nullptr)
            lua_pushnil(L);
        else {
            auto ref = lua_generic_construct<LuaActorReference>(L, "Actor");
            ref->actor = actor;
        }

        return 1;
    }

    const struct luaL_Reg lua_world_functions[] = {
            {"get_actor", lua_world_get_actor},
            {nullptr, nullptr}
    };

    int lua_open_world(lua_State* L) {
        luaL_newlib(L, lua_world_functions);
        return 1;
    }
}