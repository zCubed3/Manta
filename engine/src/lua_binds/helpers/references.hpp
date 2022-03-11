#ifndef SILICA_LUA_BINDS_REFERENCES_HPP
#define SILICA_LUA_BINDS_REFERENCES_HPP

#include "construction.hpp"

namespace Silica::LuaBindings {
    template<typename t_referee>
    class LuaBindReference {
        t_referee* ptr;

    public:
        operator t_referee*() const { return ptr; };

        static auto MakeBinding(lua_State* L, const char* table, t_referee* ptr) {
            auto ref = lua_generic_construct<LuaBindReference<t_referee>>(L, table);
            ref->ptr = ptr;
            return ref;
        }
    };
}

#endif
