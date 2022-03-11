#include "actor.hpp"

#include <iostream>

#include <world/actor.hpp>

#include "helpers/lua_helpers.hpp"

namespace Silica::LuaBindings {
    using ActorRef = LuaBindReference<Actor>;

    int lua_get_actor(lua_State* L) {
        auto actor_ref = lua_get_userdata<ActorRef*>(L);
        std::string member = lua_tostring(L, 2);

        Actor* actor = *actor_ref;

        // TODO: Lua weak refs to prevent copies
        if (member == "euler") {
            auto value = lua_generic_construct<glm::vec3>(L, "Vector3");
            *value = actor->euler;
        }

        return 1;
    }

    int lua_set_actor(lua_State* L) {
        auto actor_ref = lua_get_userdata<ActorRef*>(L);
        std::string member = lua_tostring(L, 2);

        Actor* actor = *actor_ref;

        if (member == "euler") {
            auto value = lua_get_userdata<glm::vec3*>(L, 3);
            actor->euler = *value;
        }

        return 1;
    }

    const struct luaL_Reg lua_actor_methods[] = {
            {"__index", lua_get_actor},
            {"__newindex", lua_set_actor},
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