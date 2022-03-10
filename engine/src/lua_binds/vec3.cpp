#include "vec3.hpp"

#include <string>
#include <iostream>
#include <functional>

#include <glm/vec3.hpp>

namespace Silica::LuaBindings {
    typedef glm::vec3(glm::vec3::*scalar_operator_t)(const float&) const;
    typedef glm::vec3(glm::vec3::*vector_operator_t)(const glm::vec3&) const;

    glm::vec3* lua_construct_vector3(lua_State *L) {
        auto vector3 = reinterpret_cast<glm::vec3*>(lua_newuserdata(L, sizeof(glm::vec3)));
        luaL_getmetatable(L, "Vector3");
        lua_setmetatable(L, -2);
        return vector3;
    }

    int lua_new_vector3(lua_State *L) {
        float x = luaL_checknumber(L, 1);
        float y = luaL_checknumber(L, 2);
        float z = luaL_checknumber(L, 3);

        auto vector3 = lua_construct_vector3(L);

        vector3->x = x;
        vector3->y = y;
        vector3->z = z;

        return 1;
    }

    int lua_gc_vector3(lua_State *L) {
        auto v3 = reinterpret_cast<glm::vec3*>(lua_touserdata(L, 1));
        delete v3;
        return 1;
    }

    int lua_get_vector3(lua_State* L) {
        auto v3 = reinterpret_cast<glm::vec3*>(lua_touserdata(L, 1));
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

    int lua_set_vector3(lua_State *L) {
        auto v3 = reinterpret_cast<glm::vec3*>(lua_touserdata(L, 1));
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
        auto v3 = reinterpret_cast<glm::vec3*>(lua_touserdata(L, 1));

        std::string str = "{ " + std::to_string(v3->x) + ", " + std::to_string(v3->y) + ", " + std::to_string(v3->z) + " }";
        lua_pushstring(L, str.c_str());

        return 1;
    }

    int lua_operate_vector3(lua_State *L, vector_operator_t plus_po[]) {
        auto v3 = reinterpret_cast<glm::vec3*>(lua_touserdata(L, 1));

        if (lua_isnumber(L, 2)) {
            float rhs = luaL_checknumber(L, 2);
            auto res = lua_construct_vector3(L);

            *res = std::invoke(scalar_op, v3, rhs);
        } else if (lua_istable(L, 2)) {
            auto rhs = reinterpret_cast<glm::vec3*>(lua_touserdata(L, 2));

            if (rhs == nullptr)
                throw std::runtime_error("lua_add_vector3() rhs table was nullptr!");

            auto res = lua_construct_vector3(L);

            *res = std::invoke(vector_op, v3, *rhs);
        } else {
            return 0;
        }

        return 1;
    }

    int lua_add_vector3(lua_State *L) {
        return lua_operate_vector3(L, &std::plus(), &Vector3::operator+);
    }

    int lua_sub_vector3(lua_State *L) {
        return lua_operate_vector3(L, &Vector3::operator-, &Vector3::operator-);
    }

    int lua_mul_vector3(lua_State *L) {
        return lua_operate_vector3(L, &Vector3::operator*, &Vector3::operator+);
    }

    int lua_div_vector3(lua_State *L) {
        return lua_operate_vector3(L, &Vector3::operator/, &Vector3::operator/);
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