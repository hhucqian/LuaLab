#include "luaengine.h"

#include "zlgcan/lzlgcanlib.h"
#include "propertyeditor/luakvmodel.h"
#include "lguilib.h"

#include <QApplication>
#include <QClipboard>

LuaEngine::LuaEngine(QObject *parent) : QObject(parent)
{
    this->m_L = NULL;
    this->m_lua_thread = NULL;
    this->m_msg_type_count = 0;
    connect(this, SIGNAL(ToClipboradEvent(QString)), this, SLOT(onToClipborad(QString)));
}

LuaEngine* LuaEngine::Instance()
{
    static LuaEngine *res = new LuaEngine();
    return res;
}

lua_State* LuaEngine::newLuaThread()
{
    lua_State* res = luaL_newstate();
    luaL_openlibs(res);

    luaL_requiref(res, LUA_ZLGCANNAME, luaopen_zlgcan, 0);
    lua_pop(res, 1);

    luaL_requiref(res, LUA_GUINAME, luaopen_gui, 1);
    lua_pop(res, 1);

    this->m_L = res;

    return res;
}

void LuaEngine::triggerEvent(LuaEvent &event)
{
    emit GetLuaEvent(event);
}

QThread* LuaEngine::runLuaThread(QString filename)
{
    if(this->m_lua_thread == NULL) {
        this->m_script_name = filename;
        this->m_events.clear();
        this->newLuaThread();
        this->m_lua_thread = new LuaThread(this->m_L, filename);
        this->m_lua_thread->start(QThread::HighestPriority);
        this->m_msg_type_count = 0;
        connect(this->m_lua_thread, SIGNAL(finished()), this, SLOT(onLuaThreadFinished()));
        emit LuaStateChange(this->m_script_name, true);
    }
    return this->m_lua_thread;
}

void LuaEngine::onLuaThreadFinished()
{
    this->m_lua_thread->deleteLater();
    this->m_lua_thread = NULL;
    this->closeLuaThread();
    emit LuaStateChange(this->m_script_name, false);
}

void LuaEngine::stopScript()
{
    static LuaEvent event;
    event.Type = LuaEvent::EVENT_EXIT;
    event.ExtraType = LuaEvent::EXTRA_NONE;
    gL->putEvent(event);
}

void LuaEngine::pauseScript()
{
    static LuaEvent event;
    event.Type = LuaEvent::EVENT_PAUSE;
    event.ExtraType = LuaEvent::EXTRA_INT;
    event.ExtraInt = 1;
    gL->putEvent(event);
}

void LuaEngine::resumeScript()
{
    static LuaEvent event;
    event.Type = LuaEvent::EVENT_PAUSE;
    event.ExtraType = LuaEvent::EXTRA_INT;
    event.ExtraInt = 0;
    gL->putEvent(event);
}

void LuaEngine::triggerPushMsg(int type, const char *msg)
{
    emit PushMsg(type, QString::fromUtf8(msg));
}

void LuaEngine::triggerAddMsgType(const char *name)
{
    emit AddMsgType(QString::fromUtf8(name));
}

void LuaEngine::triggerLuaScriptError(const char *msg)
{
    emit LuaScriptError(QString::fromUtf8(msg));
}

void LuaEngine::triggerSetKV(const char *key, const char *value)
{
    emit SetKVEvent(QString::fromUtf8(key), QString::fromUtf8(value));
}

int LuaEngine::getNextMsgTypeCount()
{
    ++this->m_msg_type_count;
    return this->m_msg_type_count;
}

void LuaEngine::triggetToClipboard(const char *text)
{
    emit ToClipboradEvent(QString::fromUtf8(text));
}

void LuaEngine::onToClipborad(QString text)
{
    QApplication::clipboard()->setText(text);
}
