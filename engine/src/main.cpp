#include <SDL2/SDL.h>

#include <string>
#include <iostream>
#include <sstream>

// Opengl
#include <GL/glew.h>

extern "C" { // This is to fix the C-ness of lua
    #include <lua.h>
    #include <lualib.h>
    #include <lauxlib.h>
}

// TODO: Proof of concept implementation first
// TODO: Then clean up

const char* HELLO_LUA_PATH = "lua/scripts/hello.lua";
const char* CONFIG_LUA_PATH = "lua/config/engine.lua";

class Vector2 {
public:
    float x = 0.0F;
    float y = 0.0F;

    Vector2(float x, float y) {
        this->x = x;
        this->y = y;
    }

    [[nodiscard]]
    std::string to_string() const {
        std::stringstream str;

        str << "{ " << x << ", " << y << " }";

        return str.str();
    }

    [[nodiscard]]
    std::string to_pretty_string() const {
        std::stringstream str;

        str << x << ", " << y;

        return str.str();
    }

    //
    // Addition
    //
    friend Vector2 operator+(const Vector2& lhs, const Vector2& rhs) {
        return {lhs.x + rhs.x, lhs.y + rhs.y};
    }

    friend Vector2 operator+(const Vector2& lhs, const float& rhs) {
        return {lhs.x + rhs, lhs.y + rhs};
    }

    //
    // Subtraction
    //
    friend Vector2 operator-(const Vector2& lhs, const Vector2& rhs) {
        return {lhs.x - rhs.x, lhs.y - rhs.y};
    }

    friend Vector2 operator-(const Vector2& lhs, const float& rhs) {
        return {lhs.x - rhs, lhs.y - rhs};
    }

    //
    // Multiplication
    //
    friend Vector2 operator*(const Vector2& lhs, const Vector2& rhs) {
        return {lhs.x * rhs.x, lhs.y * rhs.y};
    }

    friend Vector2 operator*(const Vector2& lhs, const float& rhs) {
        return {lhs.x * rhs, lhs.y * rhs};
    }

    //
    // Division
    //
    friend Vector2 operator/(const Vector2& lhs, const Vector2& rhs) {
        return {lhs.x / rhs.x, lhs.y / rhs.y};
    }

    friend Vector2 operator/(const Vector2& lhs, const float& rhs) {
        return {lhs.x / rhs, lhs.y / rhs};
    }

    //
    // Lua bindings
    //
protected:
    static Vector2* lua_construct_vector2(lua_State *L) {
        auto vector2 = reinterpret_cast<Vector2*>(lua_newuserdata(L, sizeof(Vector2)));
        luaL_getmetatable(L, "Vector2");
        lua_setmetatable(L, -2);
        return vector2;
    }

    static int lua_new_vector2(lua_State *L) {
        float x = luaL_checknumber(L, 1);
        float y = luaL_checknumber(L, 2);

        auto vector2 = lua_construct_vector2(L);

        vector2->x = x;
        vector2->y = y;

        return 1;
    }

    static int lua_get_vector2(lua_State *L) {
        auto v2 = reinterpret_cast<Vector2*>(lua_touserdata(L, 1));
        std::string member = lua_tostring( L, 2 );

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

    static int lua_set_vector2(lua_State *L) {
        auto v2 = reinterpret_cast<Vector2*>(lua_touserdata(L, 1));
        std::string member = lua_tostring( L, 2 );
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

    static int lua_tostring_vector2(lua_State *L) {
        auto v2 = reinterpret_cast<Vector2*>(lua_touserdata(L, 1));
        lua_pushstring(L, v2->to_string().c_str());

        return 1;
    }

    static int lua_add_vector2(lua_State *L) {
        auto v2 = reinterpret_cast<Vector2*>(lua_touserdata(L, 1));

        if (lua_isnumber(L, 2)) {
            float rhs = luaL_checknumber(L, 2);
            auto res = lua_construct_vector2(L);

            *res = *v2 + rhs;
        } else if (lua_istable(L, 2)) {
            auto rhs = reinterpret_cast<Vector2*>(lua_touserdata(L, 2));

            if (rhs == nullptr)
                throw std::runtime_error("lua_add_vector2() rhs table was nullptr!");

            auto res = lua_construct_vector2(L);

            *res = *v2 + *rhs;
        } else {
            return 0;
        }

        return 1;
    }

    static int lua_sub_vector2(lua_State *L) {
        auto v2 = reinterpret_cast<Vector2*>(lua_touserdata(L, 1));

        if (lua_isnumber(L, 2)) {
            float rhs = luaL_checknumber(L, 2);
            auto res = lua_construct_vector2(L);

            *res = *v2 - rhs;
        } else if (lua_istable(L, 2)) {
            auto rhs = reinterpret_cast<Vector2*>(lua_touserdata(L, 2));

            if (rhs == nullptr)
                throw std::runtime_error("lua_add_vector2() rhs table was nullptr!");

            auto res = lua_construct_vector2(L);

            *res = *v2 - *rhs;
        } else {
            return 0;
        }

        return 1;
    }

    static int lua_mul_vector2(lua_State *L) {
        auto v2 = reinterpret_cast<Vector2*>(lua_touserdata(L, 1));

        if (lua_isnumber(L, 2)) {
            float rhs = luaL_checknumber(L, 2);
            auto res = lua_construct_vector2(L);

            *res = *v2 * rhs;
        } else if (lua_istable(L, 2)) {
            auto rhs = reinterpret_cast<Vector2*>(lua_touserdata(L, 2));

            if (rhs == nullptr)
                throw std::runtime_error("lua_add_vector2() rhs table was nullptr!");

            auto res = lua_construct_vector2(L);

            *res = *v2 * *rhs;
        } else {
            return 0;
        }

        return 1;
    }

    static int lua_div_vector2(lua_State *L) {
        auto v2 = reinterpret_cast<Vector2*>(lua_touserdata(L, 1));

        if (lua_isnumber(L, 2)) {
            float rhs = luaL_checknumber(L, 2);
            auto res = lua_construct_vector2(L);

            *res = *v2 / rhs;
        } else if (lua_istable(L, 2)) {
            auto rhs = reinterpret_cast<Vector2*>(lua_touserdata(L, 2));

            if (rhs == nullptr)
                throw std::runtime_error("lua_add_vector2() rhs table was nullptr!");

            auto res = lua_construct_vector2(L);

            *res = *v2 / *rhs;
        } else {
            return 0;
        }

        return 1;
    }

    constexpr static const struct luaL_Reg lua_vector2_methods[] = {
            { "__index", lua_get_vector2 },
            { "__newindex", lua_set_vector2 },
            { "__tostring", lua_tostring_vector2 },
            { "__add", lua_add_vector2 },
            { "__sub", lua_sub_vector2 },
            { "__mul", lua_mul_vector2 },
            { "__div", lua_div_vector2 },
            { nullptr, nullptr }
    };

    constexpr static const struct luaL_Reg lua_vector2_functions[] = {
            { "new", lua_new_vector2 },
            { nullptr, nullptr }
    };

    // Lua register function
public:
    static int lua_open_vector2(lua_State *L) {
        luaL_newmetatable(L, "Vector2");
        lua_pushvalue(L, -1);
        lua_setfield(L, -2, "__index");
        luaL_setfuncs(L, lua_vector2_methods, 0);

        luaL_newlib(L, lua_vector2_functions);

        return 1;
    }
};

// A base behavior that has 2 variants, LuaBehavior and NativeBehavior
// LuaBehavior is self-explanatory
// NativeBehavior is a behavior defined in C++
// Something like a renderer would be a NativeBehavior!
class Behavior {
public:
    std::string type_id = "invalid"; // This is done for cross language type safety!
    int update_priority = 1000; // Everything is at 1000 by default, if this is more important shift it down!
    bool enabled = true; // Is this behavior allowed to update?

    virtual void Start() {};
    virtual void Update() {};
    virtual void PreDraw() {};
};

// You can inherit from Behavior but for safety please inherit from NativeBehavior!
class NativeBehavior : Behavior {

};

// Globals are a terrible idea but...
// Rather than creating a new Lua State per behavior they all live in the same state,
// But to work correctly we need to define a module we work in!
// Refer to silica/examples/lua_behavior for how to implement a behavior!
// Be aware, they don't load automatically, you must tell the engine to load it beforehand!
class LuaBehavior : Behavior {
public:
    LuaBehavior(lua_State* lua, const std::string& script) {
        luaL_dofile(lua, script.c_str());
    }
};

bool lua_load_script(lua_State* lua_vm, const char* path) {
    int lua_file_status = luaL_loadfile(lua_vm, path);

    if (lua_file_status != LUA_OK) {
        switch (lua_file_status) {
            default:
                break;

            case LUA_ERRFILE:
                printf("LUA: Failed to locate %s, this is fine but it is usually fine, but if more is missing the engine will not work!\n",
                       path);
                break;

            case LUA_ERRMEM:
                printf("LUA: Encountered memory error while loading %s\n", path);
                break;

            case LUA_ERRSYNTAX:
                printf("LUA: Encountered syntax error while loading %s\n", path);
                printf("LUA ERROR:\n%s\n", lua_tostring(lua_vm, -1));
                break;
        }

        return false;
    }
    else
        lua_pcall(lua_vm, 0, LUA_MULTRET, 0);

    return true;
}

// Copied from lua.c
static int lua_error_handle(lua_State* lua_vm) {
    const char *msg = lua_tostring(lua_vm, 1);
    if (msg == NULL) {  /* is error object not a string? */
        if (luaL_callmeta(lua_vm, 1, "__tostring") &&  /* does it have a metamethod */
            lua_type(lua_vm, -1) == LUA_TSTRING)  /* that produces a string? */
            return 1;  /* that is the message */
        else
            msg = lua_pushfstring(lua_vm, "(error object is a %s value)",
                                  luaL_typename(lua_vm, 1));
    }
    luaL_traceback(lua_vm, lua_vm, msg, 1);  /* append a standard traceback */
    return 1;  /* return the traceback */
}

bool lua_invoke_script(lua_State* lua_vm, int argc = 0, int retc = 0) {
    int top_pos = lua_gettop(lua_vm) - argc;

    lua_pushcfunction(lua_vm, lua_error_handle);
    lua_insert(lua_vm, top_pos);

    int run_state = lua_pcall(lua_vm, argc, retc, 0);

    if (run_state != LUA_OK) {
        printf("LUA ERROR \n%s\n", lua_tostring(lua_vm, -1));
    }

    return run_state == LUA_OK;
}

lua_State* lua_init(bool as_debug) {
    printf("LUA: Initializing Lua VM\n");
    lua_State* lua = luaL_newstate();

    // luaL_openlibs(lua_State*) is the 5.1+ replacement for
    // luaopen_base(lua_State*)
    // luaopen_table(lua_State*)
    // luaopen_io(lua_State*)
    // luaopen_string(lua_State*)
    // luaopen_math(lua_State*)

    luaL_openlibs(lua);

    // Register silica primitive types
    luaL_requiref(lua, "vector2", Vector2::lua_open_vector2, 1);

    // Enables debug mode
    if (as_debug) {
        lua_pushinteger(lua, 1);
        lua_setglobal(lua, "SILICA_LUA_DEBUG");
    }

    if (lua_load_script(lua, HELLO_LUA_PATH)) {
        lua_invoke_script(lua);
    }

    return lua;
}

class EngineConfig {
public:
    std::string title = "Silica";
    float clear_r = 0.1f, clear_g = 0.1f, clear_b = 0.1f;
    int width = 1024, height = 768;

    static EngineConfig* LoadFromLua(lua_State *L) {
        auto cfg = new EngineConfig();

        // TODO: Safety

        if (!lua_load_script(L, CONFIG_LUA_PATH))
            throw std::runtime_error("Failed loading engine config script!");

        lua_pcall(L, 0, 0, 0);

        lua_getglobal(L, "WINDOW_WIDTH");
        lua_getglobal(L, "WINDOW_HEIGHT");
        lua_getglobal(L, "WINDOW_TITLE");

        lua_getglobal(L, "CLEAR_COLOR_R");
        lua_getglobal(L, "CLEAR_COLOR_G");
        lua_getglobal(L, "CLEAR_COLOR_B");


        cfg->width = lua_tointeger(L, -6);
        cfg->height = lua_tointeger(L, -5);
        cfg->title = lua_tostring(L, -4);

        cfg->clear_r = lua_tonumber(L, -3);
        cfg->clear_g = lua_tonumber(L, -2);
        cfg->clear_b = lua_tonumber(L, -1);

        return cfg;
    }
};

int main(int argc, char** argv) {
    auto lua_vm = lua_init(true);

    auto config = EngineConfig::LoadFromLua(lua_vm);

    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_EVENTS);

    SDL_GLContext sdl_context = nullptr;
    SDL_Window* sdl_window = SDL_CreateWindow(
            config->title.c_str(),
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            config->width,
            config->height,
            SDL_WINDOW_OPENGL
    );

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

    sdl_context = SDL_GL_CreateContext(sdl_window);
    SDL_GL_SetSwapInterval(1);

    SDL_Event sdl_event;
    bool keep_running = true;

    while (keep_running) {
        // Event polling
        while (SDL_PollEvent(&sdl_event) != 0) {
            if (sdl_event.type == SDL_QUIT)
                keep_running = false;
        }

        glClearColor(config->clear_r, config->clear_g, config->clear_b, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        SDL_GL_SwapWindow(sdl_window);
    }

    SDL_DestroyWindow(sdl_window);
}