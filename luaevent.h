#ifndef LUAEVENT_H
#define LUAEVENT_H

#include <QHash>
#include <QObject>
#include <QString>

class LuaEvent : public QObject
{
    Q_OBJECT
  public:
    LuaEvent();
    enum LuaEventType
    {
        EVENT_NONE = 0,
        EVENT_FUN,
        EVENT_EXIT,
        EVENT_PAUSE,
        EVENT_SET_TEXT,
        EVENT_SET_TITLE,
        EVENT_SET_FN_TEXT,
        EVENT_END
    };
    enum LuaEventExraType
    {
        EXTRA_NONE = 0,
        EXTRA_INT,
        EXTRA_STRING,
        EXTRA_HASH,
        EXTRA_END
    };

    LuaEventType Type;
    LuaEventExraType ExtraType;
    int ExtraInt;
    QString ExtraString;
    QHash<QString, QString> ExtraHash;

    LuaEvent(const LuaEvent &e) : QObject(), Type(e.Type), ExtraType(e.ExtraType), ExtraInt(e.ExtraInt), ExtraString(e.ExtraString), ExtraHash(e.ExtraHash) {}
    LuaEvent &operator=(const LuaEvent &e);
};

#endif // LUAEVENT_H
