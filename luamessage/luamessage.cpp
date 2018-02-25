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
