#include "behavior.hpp"

namespace Silica::LuaBindings {
    int lua_new_behavior(lua_State* L) {
        return 1;
    }

    const struct luaL_Reg lua_behavior_methods[] = {
            {nullptr,      nullptr}
    };

    const struct luaL_Reg lua_behavior_functions[] = {
            {"new",   lua_new_behavior},
            {nullptr, nullptr}
    };

    int lua_open_behavior(lua_State *L) {
        luaL_newmetatable(L, "Behavior");
        lua_pushvalue(L, -1);
        lua_setfield(L, -2, "__index");
        luaL_setfuncs(L, lua_behavior_methods, 0);

        luaL_newlib(L, lua_behavior_functions);

        return 1;
    }
}