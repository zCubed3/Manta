#ifndef SILICA_VECTOR2_HPP
#define SILICA_VECTOR2_HPP

#include <string>
#include <sstream>

#include "../lua_common.hpp"

namespace Silica {
    struct Vector2 {
        float x = 0.0F;
        float y = 0.0F;

        Vector2(float x = 0, float y = 0);

        [[nodiscard]]
        std::string ToString() const;

        [[nodiscard]]
        std::string ToPrettyString() const;

        //
        // Addition
        //
        Vector2 operator+(const Vector2 &rhs) const;
        Vector2 operator+(const float &rhs) const;

        //
        // Subtraction
        //
        Vector2 operator-(const Vector2 &rhs) const;
        Vector2 operator-(const float &rhs) const;

        //
        // Multiplication
        //
        Vector2 operator*(const Vector2 &rhs) const;
        Vector2 operator*(const float &rhs) const;

        //
        // Division
        //
        Vector2 operator/(const Vector2 &rhs) const;
        Vector2 operator/(const float &rhs) const;

        //
        // Lua bindings
        //
    protected:
        static Vector2 *lua_construct_vector2(lua_State *L);

        typedef Vector2(Vector2::*scalar_operator_t)(const float&) const;
        typedef Vector2(Vector2::*vector_operator_t)(const Vector2&) const;

        static int lua_operate_vector2(lua_State *L, scalar_operator_t scalar_op, vector_operator_t vector_op);

        static int lua_new_vector2(lua_State *L);
        static int lua_get_vector2(lua_State *L);
        static int lua_set_vector2(lua_State *L);
        static int lua_tostring_vector2(lua_State *L);
        static int lua_add_vector2(lua_State *L);
        static int lua_sub_vector2(lua_State *L);
        static int lua_mul_vector2(lua_State *L);
        static int lua_div_vector2(lua_State *L);

        static const struct luaL_Reg lua_vector2_methods[];
        static const struct luaL_Reg lua_vector2_functions[];

        // Lua register function
    public:
        static int lua_open_vector2(lua_State *L);
    };
}

#endif
