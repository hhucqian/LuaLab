#include "lguilib.h"
#include "luaengine.h"

#include "sleeperthread.h"

static int gui_get_event(lua_State* L)
{
    if(!gL->hasEvent()) {
        return 0;
    }
    LuaEvent env = gL->popEvent();
    lua_pushinteger(L, env.Type);
    lua_pushinteger(L, env.ExtraType);
    switch (env.ExtraType) {
    case LuaEvent::EXTRA_NONE:
        lua_pushnil(L);
        break;
    case LuaEvent::EXTRA_INT:
        lua_pushinteger(L, env.ExtraInt);
        break;
    case LuaEvent::EXTRA_STRING:
        lua_pushstring(L, env.ExtraString.toUtf8().data());
        break;
    case LuaEvent::EXTRA_HASH:
        lua_newtable(L);
        for(QHash<QString, QString>::iterator it = env.ExtraHash.begin(); it != env.ExtraHash.end(); ++it) {
            lua_pushstring(L, it.key().toUtf8().data());
            lua_pushstring(L, it.value().toUtf8().data());
            lua_settable(L, -3);
        }
        break;
    default:
        lua_pushnil(L);
        break;
    }
    return 3;
}

static int gui_put_event(lua_State* L)
{
    static LuaEvent res;
    int type = luaL_checkinteger(L, 1);
    int etype = luaL_checkinteger(L, 2);
    if(type > LuaEvent::EVENT_NONE && type < LuaEvent::EVENT_END
            && etype >= LuaEvent::EXTRA_NONE && etype < LuaEvent::EXTRA_END) {
        res.Type =(LuaEvent::LuaEventType)type;
        res.ExtraType = (LuaEvent::LuaEventExraType)etype;
        switch (etype) {
        case LuaEvent::EXTRA_INT:
            res.ExtraInt = luaL_checkinteger(L, 3);
            break;
        case LuaEvent::EXTRA_STRING:
            res.ExtraString = QString::fromUtf8(luaL_checkstring(L, 3));
            break;
        default:
            break;
        }
        gL->triggerEvent(res);
    }
    return 0;
}

static int gui_set_title(lua_State* L)
{
    static LuaEvent res;
    res.Type = LuaEvent::EVENT_SET_TITLE;
    res.ExtraString = QString::fromUtf8(luaL_checkstring(L, 1));
    gL->triggerEvent(res);
    return 0;
}

static int gui_set_fn_title(lua_State* L)
{
    static LuaEvent res;
    res.Type = LuaEvent::EVENT_SET_FN_TEXT;
    res.ExtraInt = luaL_checkinteger(L, 1);
    res.ExtraString = QString::fromUtf8(luaL_checkstring(L, 2));
    gL->triggerEvent(res);
    return 0;
}

static int gui_getmstime(lua_State *L) {
    struct timeb tb;
    ftime(&tb);
    unsigned long res = tb.time * 1000 + tb.millitm;
    lua_pushinteger(L, res);
    return 1;
}

static int gui_sleep(lua_State *L) {
    int n = lua_tointeger(L, 1);
    if ( n < 0 ) { n = 0; }
    SleeperThread::sleep(n);
    return 0;
}

static int gui_usleep(lua_State *L) {
    int n = lua_tointeger(L, 1);
    if ( n < 0 ) { n = 0; }
    SleeperThread::usleep(n);
    return 0;
}

static int gui_msleep(lua_State *L) {
    int n = lua_tointeger(L, 1);
    if ( n < 0 ) { n = 0; }
    SleeperThread::msleep(n);
    return 0;
}

static int gui_push_msg(lua_State *L) {
    int top = lua_gettop(L);
    int type = 1;
    const char *msg = 0;
    if(top == 0) {
        luaL_argerror(L, 1, "(string) or (number ,string) expeted, got no value");
    } else if(top == 1) {
        msg = luaL_checkstring(L, 1);
        gL->triggerPushMsg(type, msg);
    } else {
        type = luaL_checkinteger(L, 1);
        msg = luaL_checkstring(L, 2);
        gL->triggerPushMsg(type, msg);
    }
    return 0;
}

static int gui_add_msg_type(lua_State *L) {
    const char *name = luaL_checkstring(L, 1);
    gL->triggerAddMsgType(name);
    return 0;
}

static int gui_set_kv(lua_State *L) {
    const char *key = luaL_checkstring(L, 1);
    const char *value = luaL_checkstring(L, 2);
    gL->triggerSetKV(key, value);
    return 0;
}

static const luaL_Reg guilib[] = {
    {"getmstime", gui_getmstime},
    {"sleep", gui_sleep},
    {"msleep", gui_msleep},
    {"usleep", gui_usleep},
    {"getevent", gui_get_event},
    {"putevent", gui_put_event},
    {"settitle", gui_set_title},
    {"setfntitle", gui_set_fn_title},
    {"addmsgtype", gui_add_msg_type},
    {"push", gui_push_msg},
    {"setkv", gui_set_kv},
    {NULL, NULL}
};


LUAMOD_API int luaopen_gui (lua_State *L) {
  luaL_newlib(L, guilib);
  lua_pushstring(L, "EVENT_NONE");
  lua_pushinteger(L, LuaEvent::EVENT_NONE);
  lua_settable(L, -3);
  lua_pushstring(L, "EVENT_FUN");
  lua_pushinteger(L, LuaEvent::EVENT_FUN);
  lua_settable(L, -3);
  lua_pushstring(L, "EVENT_EXIT");
  lua_pushinteger(L, LuaEvent::EVENT_EXIT);
  lua_settable(L, -3);
  lua_pushstring(L, "EVENT_PAUSE");
  lua_pushinteger(L, LuaEvent::EVENT_PAUSE);
  lua_settable(L, -3);
  lua_pushstring(L, "EVENT_SET_TEXT");
  lua_pushinteger(L, LuaEvent::EVENT_SET_TEXT);
  lua_settable(L, -3);
  lua_pushstring(L, "EVENT_SET_TITLE");
  lua_pushinteger(L, LuaEvent::EVENT_SET_TITLE);
  lua_settable(L, -3);
  lua_pushstring(L, "EVENT_SET_FN");
  lua_pushinteger(L, LuaEvent::EVENT_SET_FN_TEXT);
  lua_settable(L, -3);
  lua_pushstring(L, "EXTRA_NONE");
  lua_pushinteger(L, LuaEvent::EXTRA_NONE);
  lua_settable(L, -3);
  lua_pushstring(L, "EXTRA_INT");
  lua_pushinteger(L, LuaEvent::EXTRA_INT);
  lua_settable(L, -3);
  lua_pushstring(L, "EXTRA_STRING");
  lua_pushinteger(L, LuaEvent::EXTRA_STRING);
  lua_settable(L, -3);
  lua_pushstring(L, "EXTRA_HASH");
  lua_pushinteger(L, LuaEvent::EXTRA_HASH);
  lua_settable(L, -3);
  return 1;
}
