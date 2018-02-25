#ifndef LUAMESSAGE_H
#define LUAMESSAGE_H

#include <QObject>

class LuaMessage : public QObject
{
    Q_OBJECT
public:
    explicit LuaMessage(QObject *parent = 0);
    LuaMessage(int type, QString& msg): QObject(0), m_type(type), m_msg(msg){}
    LuaMessage(int type, const char *msg): QObject(0), m_type(type), m_msg(QString::fromUtf8(msg)){}
    LuaMessage(const LuaMessage& m):QObject(0), m_type(m.Type()), m_msg(m.Msg()){}
    LuaMessage& operator=(const LuaMessage& m);
    int Type() const { return this->m_type; }
    const QString& Msg() const { return this->m_msg; }

private:
    int m_type;
    QString m_msg;

signals:

public slots:
};

#endif // LUAMESSAGE_H
