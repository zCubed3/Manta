#include "vector3.hpp"

#include <iostream>
#include <functional>

#include <math.h>

namespace Silica {
    Vector3::Vector3(float x, float y, float z) {
        this->x = x;
        this->y = y;
        this->z = z;
    }

    //https://en.wikipedia.org/wiki/Fast_inverse_square_root#Accuracy
    // TODO: Faster length?
    float Vector3::Length() const {
        return sqrtf(Dot(*this)); // This is because dot(this, this) is the same as calculating raw length!
    }

    float Vector3::Dot(const Vector3& rhs) const {
        return (x * rhs.x) + (y * rhs.y) + (z * rhs.z);
    }

    Vector3 Vector3::Normalize() const {
        return *this / Length();
    }

    Vector3 Vector3::Cross(const Vector3& rhs) const {
        return {
            y * rhs.z - z * rhs.y,
            z * rhs.x - x * rhs.z,
            x * rhs.y - y * rhs.x
        };
    }

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

    int Vector3::lua_gc_vector3(lua_State *L) {
        auto v3 = reinterpret_cast<Vector3 *>(lua_touserdata(L, 1));
        free(v3);
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

    int Vector3::lua_operate_vector3(lua_State *L, scalar_operator_t scalar_op, vector_operator_t vector_op) {
        auto v3 = reinterpret_cast<Vector3 *>(lua_touserdata(L, 1));

        if (lua_isnumber(L, 2)) {
            float rhs = luaL_checknumber(L, 2);
            auto res = lua_construct_vector3(L);

            *res = std::invoke(scalar_op, v3, rhs);
        } else if (lua_istable(L, 2)) {
            auto rhs = reinterpret_cast<Vector3 *>(lua_touserdata(L, 2));

            if (rhs == nullptr)
                throw std::runtime_error("lua_add_vector3() rhs table was nullptr!");

            auto res = lua_construct_vector3(L);

            *res = std::invoke(vector_op, v3, *rhs);
        } else {
            return 0;
        }

        return 1;
    }

    int Vector3::lua_add_vector3(lua_State *L) {
        return lua_operate_vector3(L, &Vector3::operator+, &Vector3::operator+);
    }

    int Vector3::lua_sub_vector3(lua_State *L) {
        return lua_operate_vector3(L, &Vector3::operator-, &Vector3::operator-);
    }

    int Vector3::lua_mul_vector3(lua_State *L) {
        return lua_operate_vector3(L, &Vector3::operator*, &Vector3::operator+);
    }

    int Vector3::lua_div_vector3(lua_State *L) {
        return lua_operate_vector3(L, &Vector3::operator/, &Vector3::operator/);
    }

    int Vector3::lua_length_vector3(lua_State *L) {
        auto v3 = reinterpret_cast<Vector3 *>(lua_touserdata(L, 1));
        lua_pushnumber(L, v3->Length());

        return 1;
    }

    int Vector3::lua_dot_vector3(lua_State *L) {
        auto v3 = reinterpret_cast<Vector3 *>(lua_touserdata(L, 1));
        auto rhs = reinterpret_cast<Vector3 *>(lua_touserdata(L, 2));

        if (rhs == nullptr)
            return 0;

        lua_pushnumber(L, v3->Dot(*rhs));

        return 1;
    }

    int Vector3::lua_normalize_vector3(lua_State *L) {
        auto v3 = reinterpret_cast<Vector3 *>(lua_touserdata(L, 1));
        auto res = lua_construct_vector3(L);

        *res = v3->Normalize();

        return 1;
    }

    int Vector3::lua_cross_vector3(lua_State *L) {
        auto v3 = reinterpret_cast<Vector3 *>(lua_touserdata(L, 1));
        auto rhs = reinterpret_cast<Vector3 *>(lua_touserdata(L, 2));

        if (rhs == nullptr)
            return 0;

        auto res = lua_construct_vector3(L);

        *res = v3->Cross(*rhs);

        return 1;
    }

    const struct luaL_Reg Vector3::lua_vector3_methods[] = {
            {"__index",    Vector3::lua_get_vector3},
            {"__gc",       Vector3::lua_gc_vector3},
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