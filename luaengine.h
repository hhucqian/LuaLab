#ifndef LUAENGINE_H
#define LUAENGINE_H

#include <QObject>
#include <QList>
#include <QThread>
#include "Lua/lua.hpp"
#include "luaevent.h"
#include "luathread.h"

#define gL (LuaEngine::Instance())

class LuaKVModel;

class LuaEngine : public QObject
{
    Q_OBJECT
public:
    explicit LuaEngine(QObject *parent = 0);
    static LuaEngine* Instance();

    QThread* runLuaThread(QString filename);

    int hasEvent() { return this->m_events.isEmpty() ? 0 : 1; }
    int isRunning() { return this->m_lua_thread != NULL && this->m_lua_thread->isRunning(); }
    LuaEvent popEvent(){ LuaEvent res(this->m_events.first()); this->m_events.pop_front(); return res; }
    void putEvent(LuaEvent &event) { this->m_events.push_back(event); }
    void triggerEvent(LuaEvent &event);
    void triggerPushMsg(int type, const char* msg);
    void triggerAddMsgType(const char* name);
    void triggerLuaScriptError(const char* msg);
    void triggerSetKV(const char *key, const char *value);

    void stopScript();
    void pauseScript();
    void resumeScript();

signals:
    void GetLuaEvent(LuaEvent event);
    void LuaStateChange(bool isRunning);
    void PushMsg(int type, QString msg);
    void AddMsgType(QString name);
    void LuaScriptError(QString msg);
    void SetKVEvent(QString key, QString value);

private:
    QList<LuaEvent> m_events;
    lua_State* newLuaThread();
    void closeLuaThread(){ lua_close(this->m_L); this->m_L = NULL;}

    lua_State* m_L;
    LuaThread *m_lua_thread;

public slots:
    void onLuaThreadFinished();
};

#endif // LUAENGINE_H
