#define _CRT_SECURE_NO_WARNINGS

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>

#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#undef ERROR
#endif

//
// Silica giant ass prototype main.c file
// Before creating an actual structure, I'm laying out basic things I want here then actually implementing them in the future
//

// TODO: Finalize some basic structure in here then modularize it
// TODO: Make generic Lua interfaces so we can work with Lua without touching as much nitty gritty stuff
// TODO: Move math somewhere else and actually complete most of the math operations
// TODO: Help Lua resolve engine library stuff!

const char* ENGINE_CFG_PATH = "config/engine.lua";
const char* HELLO_LUA_PATH = "lua/scripts/hello.lua";

bool
lua_load_script(lua_State* lua_vm, const char* path) {
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

bool
lua_invoke_script(lua_State* lua_vm) {
    if (lua_pcall(lua_vm, 0, 0, -2) != LUA_OK) {
        printf("LUA ERROR: %s\n", lua_tostring(lua_vm, -1));
        return false;
    }

    return true;
}

lua_State*
lua_init(bool as_debug) {
    printf("LUA: Initializing Lua VM\n");
    lua_State* lua = luaL_newstate();

    // luaL_openlibs(lua_State*) is the 5.1+ replacement for
    // luaopen_base(lua_State*)
    // luaopen_table(lua_State*)
    // luaopen_io(lua_State*)
    // luaopen_string(lua_State*)
    // luaopen_math(lua_State*)

    luaL_openlibs(lua);

    // Enables debug mode
    if (as_debug) {
        lua_pushinteger(lua, 1);
        lua_setglobal(lua, "silica_lua_debug");
    }

    if (lua_load_script(lua, HELLO_LUA_PATH)) {
        lua_invoke_script(lua);
    }

    return lua;
}

// TODO: Replace me with a more useful engine config!
typedef struct silica_engine_config {
    const char* window_title;

    int width;
    int height;
} silica_engine_config_t;

void
silica_chdir(const char* path) {
#ifdef _WIN32
    SetCurrentDirectoryA(path);
#endif
}

void
silica_create_directory(const char* path) {
#ifdef _WIN32
    CreateDirectoryA(path, NULL);
#endif
}

void
silica_create_parent_directories(const char* path) {
    // We first see how many folders this path has
    int folder_count = 0;
    uint32_t path_len = strlen(path) + 1;

    char* path_dupe = reinterpret_cast<char*>(malloc(path_len));
    strcpy(path_dupe, path);

    const char* delim = "/";
    char* token = strtok(path_dupe, delim);

    while (token != NULL) {
        folder_count++;
        token = strtok(NULL, delim);
    }

    // Because we're creating parent directories, we need to validate that we aren't going over
    if (path[path_len - 2] != '/')
        folder_count--;

    // Someone passed a normal file!
    if (folder_count == 0)
        return;

    // Then actually get the folders
    char** folders = calloc(folder_count, sizeof(char*));
    strcpy(path_dupe, path);

    // Re run the tokenizer
    token = strtok(path_dupe, delim);

    for (int t = 0; t < folder_count; t++) {
        char* buffer = new char[strlen(token) + 1];
        strcpy(buffer, token);

        folders[t] = buffer;
        token = strtok(nullptr, delim);
    }

    free(path_dupe);

    // Now we follow the pattern of calling silica_create_directory(), but also chdir'ing into the new directory, then finally backing out of all at the end
    for (int t = 0; t < folder_count; t++) {
        silica_create_directory(folders[t]);
        silica_chdir(folders[t]);
    }

    for (int t = 0; t < folder_count; t++) {
        silica_chdir("..");
        free(folders[t]);
    }

    free(folders);
}

void
silica_generate_config() {
    silica_create_parent_directories(ENGINE_CFG_PATH);
    FILE* file = fopen(ENGINE_CFG_PATH, "w");

    assert(file);

    fputs("-- Silica Default Config\n\nwidth = 1280\nheight = 720\n\nwindow_title = 'Silica Window'", file);

    fclose(file);
}

silica_engine_config_t*
lua_load_engine_cfg(lua_State* lua_vm) {
    if (!lua_load_script(lua_vm, ENGINE_CFG_PATH)) {
        silica_generate_config();
        //return ERROR("Config wasn't found or contained syntax errors, check console!");
    }

    // Force the lua VM to load the values
    // NOTE: Lua won't actually load anything unless you call lua_pcall(lua_vm, 0, 0, 0), basically telling the VM to run the script once with zero return
    lua_pcall(lua_vm, 0, 0, 0);

    lua_getglobal(lua_vm, "width");
    lua_getglobal(lua_vm, "height");
    lua_getglobal(lua_vm, "window_title");

    // Because getglobal pushes to the stack we need to pop the stack for each element
    auto cfg = new silica_engine_config_t();

    cfg->width = lua_tonumber(lua_vm, -3);
    cfg->height = lua_tonumber(lua_vm, -2);
    cfg->window_title = lua_tostring(lua_vm, -1);

    return cfg;
}

int
main(int argc, char** argv) {
    lua_State* lua_vm = lua_init(true);
    optional_error_t cfg_result = lua_load_engine_cfg(lua_vm);

    if (HAS_ERROR(&cfg_result)) {
        printf("Encountered an error while loading engine config!\n");
        error_handle(&cfg_result);
    }

    silica_engine_config_t* cfg = cfg_result.object;

    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_EVENTS);

    SDL_Window* sdl_window = SDL_CreateWindow(
            cfg->window_title,
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            cfg->width,
            cfg->height,
            SDL_WINDOW_VULKAN
    );

    SDL_Event sdl_event;
    bool keep_running = true;

    while (keep_running) {
        // Event polling
        while (SDL_PollEvent(&sdl_event) != 0) {
            if (sdl_event.type == SDL_QUIT)
                keep_running = false;
        }
    }

    return 0;
}