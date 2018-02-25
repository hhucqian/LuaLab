#include "luaevent.h"

LuaEvent::LuaEvent() : QObject(), ExtraInt(0)
{

}

LuaEvent &LuaEvent::operator=(const LuaEvent &e)
{
    this->Type = e.Type;
    this->ExtraType = e.ExtraType;
    this->ExtraInt = e.ExtraInt;
    this->ExtraString = e.ExtraString;
    this->ExtraHash = e.ExtraHash;
    return *this;
}
