#include "world.hpp"

#include <iostream>

#include <world/actor.hpp>
#include <world/world.hpp>

#include "actor.hpp"

#include "helpers/lua_helpers.hpp"

namespace Silica::LuaBindings {
    int lua_get_actor_world(lua_State* L) {
        std::string name = luaL_checkstring(L, -1);
        Actor* actor = World::FindActor(name);

        if (actor == nullptr)
            lua_pushnil(L);
        else
            LuaBindReference<Actor>::MakeBinding(L, "Actor", actor);

        return 1;
    }

    const struct luaL_Reg lua_world_functions[] = {
            {"get_actor", lua_get_actor_world},
            {nullptr, nullptr}
    };

    int lua_open_world(lua_State* L) {
        luaL_newlib(L, lua_world_functions);
        return 1;
    }
}