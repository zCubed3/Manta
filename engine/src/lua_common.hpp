#ifndef SILICA_LUA_COMMON_HPP
#define SILICA_LUA_COMMON_HPP

extern "C" { // This is to fix the C-ness of lua
    #include <lua.h>
    #include <lualib.h>
    #include <lauxlib.h>
}

#define LUA_HELLO_PATH "lua/scripts/hello.lua"
#define LUA_ENGINE_CFG_PATH "lua/config/engine.lua"

#endif
