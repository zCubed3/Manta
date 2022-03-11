#ifndef SILICA_LUA_BINDS_CONSTRUCTION_HPP
#define SILICA_LUA_BINDS_CONSTRUCTION_HPP

namespace Silica::LuaBindings {
    template<typename target>
    static target* lua_generic_construct(lua_State* L, const char* table) {
        auto res = reinterpret_cast<target*>(lua_newuserdata(L, sizeof(target)));
        luaL_getmetatable(L, table);
        lua_setmetatable(L, -2);
        return res;
    }

    template<typename target>
    static target lua_get_userdata(lua_State* L, int idx = 1) {
        return reinterpret_cast<target>(lua_touserdata(L, idx));
    }
}

#endif
