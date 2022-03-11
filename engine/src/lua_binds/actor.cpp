#include "actor.hpp"

#include <iostream>

#include <world/actor.hpp>

#include "lua_helpers.hpp"

namespace Silica::LuaBindings {
    int lua_actor_get(lua_State* L) {
        auto actor = lua_get_userdata<LuaActorReference*>(L);
        std::string member = lua_tostring(L, 2);

        // TODO: Lua weak refs to prevent copies
        if (member == "euler") {
            auto value = lua_generic_construct<glm::vec3>(L, "Vector3");
            *value = actor->actor->euler;
        }

        return 1;
    }

    int lua_actor_set(lua_State* L) {
        auto actor = lua_get_userdata<LuaActorReference*>(L);
        std::string member = lua_tostring(L, 2);

        if (member == "euler") {
            auto value = lua_get_userdata<glm::vec3*>(L, 3);
            actor->actor->euler = *value;
        }

        return 1;
    }

    const struct luaL_Reg lua_actor_methods[] = {
            {"__index", lua_actor_get},
            {"__newindex", lua_actor_set},
            {nullptr, nullptr}
    };

    const struct luaL_Reg lua_actor_functions[] = {
            {nullptr, nullptr}
    };

    int lua_open_actor(lua_State* L) {
        luaL_newmetatable(L, "Actor");
        lua_pushvalue(L, -1);
        lua_setfield(L, -2, "__index");
        luaL_setfuncs(L, lua_actor_methods, 0);

        luaL_newlib(L, lua_actor_functions);

        return 1;
    }
}