#ifndef LGUILIB_H
#define LGUILIB_H

#include "Lua/lua.hpp"

#define LUA_GUINAME "gui"
LUAMOD_API int(luaopen_gui)(lua_State *L);

#endif // LGUILIB_H
