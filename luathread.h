#ifndef LUATHREAD_H
#define LUATHREAD_H

#include <QThread>
#include <lua/lua.hpp>

class LuaThread : public QThread
{
    Q_OBJECT
  public:
    explicit LuaThread(lua_State *L, QString file);

  signals:

  protected:
    void run();

  public slots:

  private:
    lua_State *m_L;
    QString m_filename;
};

#endif // LUATHREAD_H
