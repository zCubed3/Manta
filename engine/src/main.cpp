#include <SDL2/SDL.h>

#include <string>
#include <iostream>
#include <sstream>

// Opengl
#include <GL/glew.h>

// Types
#include "types/vector2.hpp"
#include "types/vector3.hpp"
#include "types/vector4.hpp"
#include "types/matrix4x4.hpp"

#include "rendering/mesh.hpp"
#include "rendering/shader.hpp"
#include "rendering/material.hpp"

// TODO: Proof of concept implementation first
// TODO: Then clean up

#include "lua_common.hpp"

using namespace Silica;

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

    int run_state = lua_pcall(lua_vm, argc, retc, top_pos);

    if (run_state != LUA_OK) {
        printf("LUA ERROR:\n%s\n", lua_tostring(lua_vm, -1));
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
    luaL_requiref(lua, "vector3", Vector3::lua_open_vector3, 1);

    // Enables debug mode
    if (as_debug) {
        lua_pushinteger(lua, 1);
        lua_setglobal(lua, "SILICA_LUA_DEBUG");
    }

    if (lua_load_script(lua, LUA_HELLO_PATH)) {
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

        if (!lua_load_script(L, LUA_ENGINE_CFG_PATH))
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

    Shader* shader = Shader::LoadFile("shaders/unlit.glsl");

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