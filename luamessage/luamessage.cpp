#include "luamessage.h"

LuaMessage::LuaMessage(QObject *parent) : QObject(parent)
{

}

LuaMessage& LuaMessage::operator =(const LuaMessage &m)
{
    this->m_msg = m.Msg();
    this->m_type = m.Type();
    return *this;
}

bool LuaMessage::isExpire(int sec) const
{
    bool res = false;
    if( QDateTime::currentSecsSinceEpoch() - this->m_datetime.toSecsSinceEpoch() > sec){
        res= true;
    }
    return res;
}
