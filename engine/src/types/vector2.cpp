#include "vector2.hpp"

#include <iostream>
#include <functional>

namespace Silica {
    Vector2::Vector2(float x, float y) {
        this->x = x;
        this->y = y;
    }

    [[nodiscard]]
    std::string Vector2::ToString() const {
        std::stringstream str;

        str << "{ " << x << ", " << y << " }";

        return str.str();
    }

    [[nodiscard]]
    std::string Vector2::ToPrettyString() const {
        std::stringstream str;

        str << x << ", " << y;

        return str.str();
    }

    //
    // Addition
    //
    Vector2 Vector2::operator+(const Vector2 &rhs) const {
        return {x + rhs.x, y + rhs.y};
    }

    Vector2 Vector2::operator+(const float &rhs) const {
        return {x + rhs, y + rhs};
    }

    //
    // Subtraction
    //
    Vector2 Vector2::operator-(const Vector2 &rhs) const {
        return {x - rhs.x, y - rhs.y};
    }

    Vector2 Vector2::operator-(const float &rhs) const {
        return {x - rhs, y - rhs};
    }

    //
    // Multiplication
    //
    Vector2 Vector2::operator*(const Vector2 &rhs) const {
        return {x * rhs.x, y * rhs.y};
    }

    Vector2 Vector2::operator*(const float &rhs) const {
        return {x * rhs, y * rhs};
    }

    //
    // Division
    //
    Vector2 Vector2::operator/(const Vector2 &rhs) const {
        return {x / rhs.x, y / rhs.y};
    }

    Vector2 Vector2::operator/(const float &rhs) const {
        return {x / rhs, y / rhs};
    }

    //
    // Lua bindings
    //
    Vector2 *Vector2::lua_construct_vector2(lua_State *L) {
        auto vector2 = reinterpret_cast<Vector2 *>(lua_newuserdata(L, sizeof(Vector2)));
        luaL_getmetatable(L, "Vector2");
        lua_setmetatable(L, -2);
        return vector2;
    }

    int Vector2::lua_new_vector2(lua_State *L) {
        float x = luaL_checknumber(L, 1);
        float y = luaL_checknumber(L, 2);

        auto vector2 = lua_construct_vector2(L);

        vector2->x = x;
        vector2->y = y;

        return 1;
    }

    int Vector2::lua_get_vector2(lua_State *L) {
        auto v2 = reinterpret_cast<Vector2 *>(lua_touserdata(L, 1));
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

    int Vector2::lua_set_vector2(lua_State *L) {
        auto v2 = reinterpret_cast<Vector2 *>(lua_touserdata(L, 1));
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

    int Vector2::lua_tostring_vector2(lua_State *L) {
        auto v2 = reinterpret_cast<Vector2 *>(lua_touserdata(L, 1));
        lua_pushstring(L, v2->ToString().c_str());

        return 1;
    }

    int Vector2::lua_operate_vector2(lua_State *L, scalar_operator_t scalar_op, vector_operator_t vector_op) {
        auto v2 = reinterpret_cast<Vector2 *>(lua_touserdata(L, 1));

        if (lua_isnumber(L, 2)) {
            float rhs = luaL_checknumber(L, 2);
            auto res = lua_construct_vector2(L);

            *res = std::invoke(scalar_op, v2, rhs);
        } else if (lua_istable(L, 2)) {
            auto rhs = reinterpret_cast<Vector2 *>(lua_touserdata(L, 2));

            if (rhs == nullptr)
                throw std::runtime_error("lua_add_vector2() rhs table was nullptr!");

            auto res = lua_construct_vector2(L);

            *res = std::invoke(vector_op, v2, *rhs);
        } else {
            return 0;
        }

        return 1;
    }

    int Vector2::lua_add_vector2(lua_State *L) {
        return lua_operate_vector2(L, &Vector2::operator+, &Vector2::operator+);
    }

    int Vector2::lua_sub_vector2(lua_State *L) {
        return lua_operate_vector2(L, &Vector2::operator-, &Vector2::operator-);
    }

    int Vector2::lua_mul_vector2(lua_State *L) {
        return lua_operate_vector2(L, &Vector2::operator*, &Vector2::operator+);
    }

    int Vector2::lua_div_vector2(lua_State *L) {
        return lua_operate_vector2(L, &Vector2::operator/, &Vector2::operator/);
    }

    const struct luaL_Reg Vector2::lua_vector2_methods[] = {
            {"__index",    Vector2::lua_get_vector2},
            {"__newindex", Vector2::lua_set_vector2},
            {"__tostring", Vector2::lua_tostring_vector2},
            {"__add",      Vector2::lua_add_vector2},
            {"__sub",      Vector2::lua_sub_vector2},
            {"__mul",      Vector2::lua_mul_vector2},
            {"__div",      Vector2::lua_div_vector2},
            {nullptr,      nullptr}
    };

    const struct luaL_Reg Vector2::lua_vector2_functions[] = {
            {"new",   Vector2::lua_new_vector2},
            {nullptr, nullptr}
    };

    int Vector2::lua_open_vector2(lua_State *L) {
        luaL_newmetatable(L, "Vector2");
        lua_pushvalue(L, -1);
        lua_setfield(L, -2, "__index");
        luaL_setfuncs(L, lua_vector2_methods, 0);

        luaL_newlib(L, lua_vector2_functions);

        return 1;
    }
}