#include "vec2.hpp"

#include <glm/vec2.hpp>

#include <iostream>
#include <string>

#include "helpers/lua_helpers.hpp"

namespace Silica::LuaBindings {
    typedef glm::vec2(*scalar_operator_t)(glm::vec2&, const float&);
    typedef glm::vec2(*vector_operator_t)(glm::vec2&, const glm::vec2&);

    #define LUA_MAKE_VECTOR2(L) lua_generic_construct<glm::vec2>(L, "Vector2");

    int lua_new_vector2(lua_State *L) {
        float x = luaL_checknumber(L, 1);
        float y = luaL_checknumber(L, 2);

        auto vector2 = LUA_MAKE_VECTOR2(L);

        vector2->x = x;
        vector2->y = y;

        return 1;
    }

    int lua_gc_vector2(lua_State *L) {
        //delete lua_get_userdata<glm::vec2*>(L);
        return 1;
    }

    int lua_get_vector2(lua_State *L) {
        auto v2 = lua_get_userdata<glm::vec2*>(L);
        std::string member = lua_tostring(L, 2);

        if (member == "x") {
            lua_pushnumber(L, v2->x);
        } else if (member == "y") {
            lua_pushnumber(L, v2->y);
        } else {
#ifdef DEBUG
            std::cout << "LUA: Get '" << member << "' was called!" << std::endl;
#endif
            return 0;
        }

        return 1;
    }

    int lua_tostring_vector2(lua_State *L) {
        auto v2 = lua_get_userdata<glm::vec2 *>(L);

        std::string str = "{ " + std::to_string(v2->x) + ", " + std::to_string(v2->y) + " }";
        lua_pushstring(L, str.c_str());

        return 1;
    }

    int lua_operate_vector2(lua_State *L, vector_operator_t vector_op, scalar_operator_t scalar_op) {
        auto v2 = lua_get_userdata<glm::vec2*>(L);

        if (lua_isnumber(L, 2)) {
            float rhs = luaL_checknumber(L, 2);
            auto res = LUA_MAKE_VECTOR2(L);

            *res = scalar_op(*v2, rhs);
        } else if (lua_istable(L, 2)) {
            auto rhs = lua_get_userdata<glm::vec2*>(L, 2);

            if (rhs == nullptr)
                throw std::runtime_error("lua_add_vector2() rhs table was nullptr!");

            auto res = LUA_MAKE_VECTOR2(L);
            *res = vector_op(*v2, *rhs);
        } else {
            return 0;
        }

        return 1;
    }

    int lua_add_vector2(lua_State *L) {
        // GLM doesn't expose operators nicely!
        return lua_operate_vector2(L,
            [](glm::vec2& a, const glm::vec2& b) { return a + b; },
            [](glm::vec2& a, const float& b) { return a + b; }
        );
    }

    int lua_sub_vector2(lua_State *L) {
        return lua_operate_vector2(L,
            [](glm::vec2& a, const glm::vec2& b) { return a - b; },
            [](glm::vec2& a, const float& b) { return a - b; }
        );
    }

    int lua_mul_vector2(lua_State *L) {
        return lua_operate_vector2(L,
            [](glm::vec2& a, const glm::vec2& b) { return a * b; },
            [](glm::vec2& a, const float& b) { return a * b; }
        );
    }

    int lua_div_vector2(lua_State *L) {
        return lua_operate_vector2(L,
            [](glm::vec2& a, const glm::vec2& b) { return a / b; },
            [](glm::vec2& a, const float& b) { return a / b; }
        );
    }

    int lua_set_vector2(lua_State *L) {
        auto v2 = lua_get_userdata<glm::vec2*>(L);
        std::string member = lua_tostring(L, 2);
        float value = luaL_checknumber(L, 3);

        if (member == "x") {
            v2->x = value;
        } else if (member == "y") {
            v2->y = value;
        } else {
#ifdef DEBUG
            std::cout << "LUA: Set '" << member << "' was called!" << std::endl;
#endif
            return 0;
        }

        return 1;
    }

    const struct luaL_Reg lua_vector2_methods[] = {
            {"__index",    lua_get_vector2},
            {"__gc",       lua_gc_vector2},
            {"__newindex", lua_set_vector2},
            {"__tostring", lua_tostring_vector2},
            {"__add",      lua_add_vector2},
            {"__sub",      lua_sub_vector2},
            {"__mul",      lua_mul_vector2},
            {"__div",      lua_div_vector2},
            {nullptr,      nullptr}
    };

    const struct luaL_Reg lua_vector2_functions[] = {
            {"new",   lua_new_vector2},
            {nullptr, nullptr}
    };

    int lua_open_vector2(lua_State* L) {
        luaL_newmetatable(L, "Vector2");
        lua_pushvalue(L, -1);
        lua_setfield(L, -2, "__index");
        luaL_setfuncs(L, lua_vector2_methods, 0);

        luaL_newlib(L, lua_vector2_functions);

        return 1;
    }
}