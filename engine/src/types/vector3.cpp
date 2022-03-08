#include "vector3.hpp"

#include <iostream>

namespace Silica {
    //
    // Addition
    //
    Vector3 Vector3::operator+(const Vector3 &rhs) const {
        return {x + rhs.x, y + rhs.y, z + rhs.z};
    }

    Vector3 Vector3::operator+(const float &rhs) const {
        return {x + rhs, y + rhs, z + rhs};
    }

    //
    // Subtraction
    //
    Vector3 Vector3::operator-(const Vector3 &rhs) const {
        return {x - rhs.x, y - rhs.y, z - rhs.z};
    }

    Vector3 Vector3::operator-(const float &rhs) const {
        return {x - rhs, y - rhs, z - rhs};
    }

    //
    // Multiplication
    //
    Vector3 Vector3::operator*(const Vector3 &rhs) const {
        return {x * rhs.x, y * rhs.y, z * rhs.z};
    }

    Vector3 Vector3::operator*(const float &rhs) const {
        return {x * rhs, y * rhs, z * rhs};
    }

    //
    // Division
    //
    Vector3 Vector3::operator/(const Vector3 &rhs) const {
        return {x / rhs.x, y / rhs.y, z / rhs.z};
    }

    Vector3 Vector3::operator/(const float &rhs) const {
        return {x / rhs, y / rhs, z / rhs};
    }

    //
    // Lua bindings
    //
    Vector3 *Vector3::lua_construct_vector3(lua_State *L) {
        auto vector3 = reinterpret_cast<Vector3 *>(lua_newuserdata(L, sizeof(Vector3)));
        luaL_getmetatable(L, "Vector3");
        lua_setmetatable(L, -2);
        return vector3;
    }

    int Vector3::lua_new_vector3(lua_State *L) {
        float x = luaL_checknumber(L, 1);
        float y = luaL_checknumber(L, 2);
        float z = luaL_checknumber(L, 3);

        auto vector3 = lua_construct_vector3(L);

        vector3->x = x;
        vector3->y = y;
        vector3->z = z;

        return 1;
    }

    int Vector3::lua_get_vector3(lua_State *L) {
        auto v3 = reinterpret_cast<Vector3 *>(lua_touserdata(L, 1));
        std::string member = lua_tostring(L, 2);

        if (member == "x") {
            lua_pushnumber(L, v3->x);
        } else if (member == "y") {
            lua_pushnumber(L, v3->y);
        } else if (member == "z") {
            lua_pushnumber(L, v3->z);
        } else {
#ifdef DEBUG
            std::cout << "LUA: Get '" << member << "' was called!" << std::endl;
#endif
            return 0;
        }

        return 1;
    }

    int Vector3::lua_set_vector3(lua_State *L) {
        auto v3 = reinterpret_cast<Vector3 *>(lua_touserdata(L, 1));
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

    int Vector3::lua_tostring_vector3(lua_State *L) {
        auto v3 = reinterpret_cast<Vector3 *>(lua_touserdata(L, 1));
        lua_pushstring(L, v3->to_string().c_str());

        return 1;
    }

    int Vector3::lua_add_vector3(lua_State *L) {
        auto v3 = reinterpret_cast<Vector3 *>(lua_touserdata(L, 1));

        if (lua_isnumber(L, 2)) {
            float rhs = luaL_checknumber(L, 2);
            auto res = lua_construct_vector3(L);

            *res = *v3 + rhs;
        } else if (lua_istable(L, 2)) {
            auto rhs = reinterpret_cast<Vector3 *>(lua_touserdata(L, 2));

            if (rhs == nullptr)
                throw std::runtime_error("lua_add_vector3() rhs table was nullptr!");

            auto res = lua_construct_vector3(L);

            *res = *v3 + *rhs;
        } else {
            return 0;
        }

        return 1;
    }

    int Vector3::lua_sub_vector3(lua_State *L) {
        auto v3 = reinterpret_cast<Vector3 *>(lua_touserdata(L, 1));

        if (lua_isnumber(L, 2)) {
            float rhs = luaL_checknumber(L, 2);
            auto res = lua_construct_vector3(L);

            *res = *v3 - rhs;
        } else if (lua_istable(L, 2)) {
            auto rhs = reinterpret_cast<Vector3 *>(lua_touserdata(L, 2));

            if (rhs == nullptr)
                throw std::runtime_error("lua_add_vector3() rhs table was nullptr!");

            auto res = lua_construct_vector3(L);

            *res = *v3 - *rhs;
        } else {
            return 0;
        }

        return 1;
    }

    int Vector3::lua_mul_vector3(lua_State *L) {
        auto v3 = reinterpret_cast<Vector3 *>(lua_touserdata(L, 1));

        if (lua_isnumber(L, 2)) {
            float rhs = luaL_checknumber(L, 2);
            auto res = lua_construct_vector3(L);

            *res = *v3 * rhs;
        } else if (lua_istable(L, 2)) {
            auto rhs = reinterpret_cast<Vector3 *>(lua_touserdata(L, 2));

            if (rhs == nullptr)
                throw std::runtime_error("lua_add_vector3() rhs table was nullptr!");

            auto res = lua_construct_vector3(L);

            *res = *v3 * *rhs;
        } else {
            return 0;
        }

        return 1;
    }

    int Vector3::lua_div_vector3(lua_State *L) {
        auto v3 = reinterpret_cast<Vector3 *>(lua_touserdata(L, 1));

        if (lua_isnumber(L, 2)) {
            float rhs = luaL_checknumber(L, 2);
            auto res = lua_construct_vector3(L);

            *res = *v3 / rhs;
        } else if (lua_istable(L, 2)) {
            auto rhs = reinterpret_cast<Vector3 *>(lua_touserdata(L, 2));

            if (rhs == nullptr)
                throw std::runtime_error("lua_add_vector3() rhs table was nullptr!");

            auto res = lua_construct_vector3(L);

            *res = *v3 / *rhs;
        } else {
            return 0;
        }

        return 1;
    }

    const struct luaL_Reg Vector3::lua_vector3_methods[] = {
            {"__index",    Vector3::lua_get_vector3},
            {"__newindex", Vector3::lua_set_vector3},
            {"__tostring", Vector3::lua_tostring_vector3},
            {"__add",      Vector3::lua_add_vector3},
            {"__sub",      Vector3::lua_sub_vector3},
            {"__mul",      Vector3::lua_mul_vector3},
            {"__div",      Vector3::lua_div_vector3},
            {nullptr,      nullptr}
    };

    const struct luaL_Reg Vector3::lua_vector3_functions[] = {
            {"new",   Vector3::lua_new_vector3},
            {nullptr, nullptr}
    };

    int Vector3::lua_open_vector3(lua_State *L) {
        luaL_newmetatable(L, "Vector3");
        lua_pushvalue(L, -1);
        lua_setfield(L, -2, "__index");
        luaL_setfuncs(L, lua_vector3_methods, 0);

        luaL_newlib(L, lua_vector3_functions);

        return 1;
    }
}