#include "luathread.h"
#include "luaengine.h"
#include <QDebug>

LuaThread::LuaThread(lua_State *L, QString filename) : QThread(), m_L(L), m_filename(filename)
{
    this->setTerminationEnabled(true);
}

void LuaThread::run()
{
    int n = luaL_dofile(this->m_L, this->m_filename.toUtf8().data());
        if(n) {
            gL->triggerLuaScriptError(lua_tostring(this->m_L, -1));
            lua_pop(this->m_L, 1);
        }
}


