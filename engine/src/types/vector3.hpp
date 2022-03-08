#ifndef SILICA_VECTOR3_HPP
#define SILICA_VECTOR3_HPP

#include <string>
#include <sstream>

#include "../lua_common.hpp"

namespace Silica {
    struct Vector3 {
        float x = 0.0F;
        float y = 0.0F;
        float z = 0.0F;

        Vector3(float x = 0, float y = 0, float z = 0);

        [[nodiscard]]
        std::string to_string() const {
            std::stringstream str;

            str << "{ " << x << ", " << y << ", " << z << " }";

            return str.str();
        }

        [[nodiscard]]
        std::string to_pretty_string() const {
            std::stringstream str;

            str << x << ", " << y << ", " << z;

            return str.str();
        }

        //
        // Vector math
        //
        [[nodiscard]] float Length() const;
        [[nodiscard]] float Dot(const Vector3& rhs) const;

        [[nodiscard]] Vector3 Normalize() const;
        [[nodiscard]] Vector3 Cross(const Vector3& rhs) const;

        //
        // Addition
        //
        Vector3 operator+(const Vector3 &rhs) const;
        Vector3 operator+(const float &rhs) const;

        //
        // Subtraction
        //
        Vector3 operator-(const Vector3 &rhs) const;
        Vector3 operator-(const float &rhs) const;

        //
        // Multiplication
        //
        Vector3 operator*(const Vector3 &rhs) const;
        Vector3 operator*(const float &rhs) const;

        //
        // Division
        //
        Vector3 operator/(const Vector3 &rhs) const;
        Vector3 operator/(const float &rhs) const;

        //
        // Lua bindings
        //
    protected:
        static Vector3 *lua_construct_vector3(lua_State *L);

        static int lua_new_vector3(lua_State *L);
        static int lua_gc_vector3(lua_State *L);

        static int lua_get_vector3(lua_State *L);
        static int lua_set_vector3(lua_State *L);

        static int lua_tostring_vector3(lua_State *L);

        static int lua_add_vector3(lua_State *L);
        static int lua_sub_vector3(lua_State *L);
        static int lua_mul_vector3(lua_State *L);
        static int lua_div_vector3(lua_State *L);

        static int lua_length_vector3(lua_State *L);
        static int lua_dot_vector3(lua_State *L);
        static int lua_normalize_vector3(lua_State *L);
        static int lua_cross_vector3(lua_State *L);

        static const struct luaL_Reg lua_vector3_methods[];
        static const struct luaL_Reg lua_vector3_functions[];

        // Lua register function
    public:
        static int lua_open_vector3(lua_State *L);
    };
}

#endif
