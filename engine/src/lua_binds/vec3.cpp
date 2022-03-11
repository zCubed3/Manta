#include "vec3.hpp"

#include <string>
#include <iostream>
#include <functional>

#include <glm/vec3.hpp>
#include <glm/geometric.hpp>

#include "helpers/lua_helpers.hpp"

namespace Silica::LuaBindings {
    typedef glm::vec3(*scalar_operator_t)(glm::vec3&, const float&);
    typedef glm::vec3(*vector_operator_t)(glm::vec3&, const glm::vec3&);

    #define LUA_MAKE_VECTOR3(L) lua_generic_construct<glm::vec3>(L, "Vector3");

    int lua_new_vector3(lua_State *L) {
        float x = luaL_checknumber(L, 1);
        float y = luaL_checknumber(L, 2);
        float z = luaL_checknumber(L, 3);

        auto vector3 = LUA_MAKE_VECTOR3(L);

        vector3->x = x;
        vector3->y = y;
        vector3->z = z;

        return 1;
    }

    int lua_gc_vector3(lua_State *L) {
        //delete lua_get_userdata<glm::vec3*>(L);
        return 1;
    }

    int lua_set_vector3(lua_State *L) {
        auto v3 = lua_get_userdata<glm::vec3*>(L);
        std::string member = lua_tostring(L, 2);
        float value = luaL_checknumber(L, 3);

        if (member == "x") {
            v3->x = value;
        } else if (member == "y") {
            v3->y = value;
        } else if (member == "z") {
            v3->z = value;
        } else {
#ifdef DEBUG
            std::cout << "LUA: Set '" << member << "' was called!" << std::endl;
#endif
            return 0;
        }

        return 1;
    }

    int lua_tostring_vector3(lua_State *L) {
        auto v3 = lua_get_userdata<glm::vec3*>(L);

        std::string str = "{ " + std::to_string(v3->x) + ", " + std::to_string(v3->y) + ", " + std::to_string(v3->z) + " }";
        lua_pushstring(L, str.c_str());

        return 1;
    }

    int lua_operate_vector3(lua_State *L, vector_operator_t vector_op, scalar_operator_t scalar_op) {
        auto v3 = lua_get_userdata<glm::vec3*>(L);

        if (lua_isnumber(L, 2)) {
            float rhs = luaL_checknumber(L, 2);
            auto res = LUA_MAKE_VECTOR3(L);

            *res = scalar_op(*v3, rhs);
        } else {
            auto rhs = reinterpret_cast<glm::vec3 *>(lua_touserdata(L, 2));

            if (rhs == nullptr)
                throw std::runtime_error("lua_add_vector3() rhs table was nullptr!");

            auto res = LUA_MAKE_VECTOR3(L);
            *res = vector_op(*v3, *rhs);
        }

        return 1;
    }

    int lua_add_vector3(lua_State *L) {
        // GLM doesn't expose operators nicely!
        return lua_operate_vector3(L,
            [](glm::vec3& a, const glm::vec3& b) { return a + b; },
            [](glm::vec3& a, const float& b) { return a + b; }
        );
    }

    int lua_sub_vector3(lua_State *L) {
        return lua_operate_vector3(L,
            [](glm::vec3& a, const glm::vec3& b) { return a - b; },
            [](glm::vec3& a, const float& b) { return a - b; }
        );
    }

    int lua_mul_vector3(lua_State *L) {
        return lua_operate_vector3(L,
            [](glm::vec3& a, const glm::vec3& b) { return a * b; },
            [](glm::vec3& a, const float& b) { return a * b; }
        );
    }

    int lua_div_vector3(lua_State *L) {
        return lua_operate_vector3(L,
            [](glm::vec3& a, const glm::vec3& b) { return a / b; },
            [](glm::vec3& a, const float& b) { return a / b; }
        );
    }

    //
    // TODO: Find a way to make things like this more generic
    //
    int lua_length_vector3(lua_State *L) {
        auto v3 = lua_get_userdata<glm::vec3*>(L);
        lua_pushnumber(L, glm::length(*v3));

        return 1;
    }

    int lua_dot_vector3(lua_State *L) {
        auto v3 = lua_get_userdata<glm::vec3*>(L);
        auto rhs = lua_get_userdata<glm::vec3*>(L, 2);

        if (rhs == nullptr)
            return 0;

        lua_pushnumber(L, glm::dot(*v3, *rhs));

        return 1;
    }

    int lua_normalize_vector3(lua_State *L) {
        auto v3 = lua_get_userdata<glm::vec3*>(L);
        auto res = LUA_MAKE_VECTOR3(L);

        *res = glm::normalize(*v3);
        return 1;
    }

    int lua_cross_vector3(lua_State *L) {
        auto v3 = lua_get_userdata<glm::vec3*>(L);
        auto rhs = lua_get_userdata<glm::vec3*>(L, 2);

        if (rhs == nullptr)
            return 0;

        auto res = LUA_MAKE_VECTOR3(L);
        *res = glm::cross(*v3, *rhs);

        return 1;
    }

    int lua_get_vector3(lua_State* L) {
        auto v3 = lua_get_userdata<glm::vec3*>(L);
        std::string member = lua_tostring(L, 2);

        if (member == "x") {
            lua_pushnumber(L, v3->x);
        } else if (member == "y") {
            lua_pushnumber(L, v3->y);
        } else if (member == "z") {
            lua_pushnumber(L, v3->z);
        }
            //
            // Methods
            //
        else if (member == "length") {
            lua_pushcfunction(L, lua_length_vector3);
        } else if (member == "dot") {
            lua_pushcfunction(L, lua_dot_vector3);
        } else if (member == "normalize") {
            lua_pushcfunction(L, lua_normalize_vector3);
        } else if (member == "cross") {
            lua_pushcfunction(L, lua_cross_vector3);
        }
            //
            // Fallback
            //
        else {
#ifdef DEBUG
            std::cout << "LUA: Get '" << member << "' was called!" << std::endl;
#endif
            return 0;
        }

        return 1;
    }

    const struct luaL_Reg lua_vector3_methods[] = {
            {"__index",    lua_get_vector3},
            {"__gc",       lua_gc_vector3},
            {"__newindex", lua_set_vector3},
            {"__tostring", lua_tostring_vector3},
            {"__add",      lua_add_vector3},
            {"__sub",      lua_sub_vector3},
            {"__mul",      lua_mul_vector3},
            {"__div",      lua_div_vector3},
            {nullptr,      nullptr}
    };

    const struct luaL_Reg lua_vector3_functions[] = {
            {"new",   lua_new_vector3},
            {nullptr, nullptr}
    };

    int lua_open_vector3(lua_State *L) {
        luaL_newmetatable(L, "Vector3");
        lua_pushvalue(L, -1);
        lua_setfield(L, -2, "__index");
        luaL_setfuncs(L, lua_vector3_methods, 0);

        luaL_newlib(L, lua_vector3_functions);

        return 1;
    }
}