#-------------------------------------------------
#
# Project created by QtCreator 2018-02-17T15:34:52
#
#-------------------------------------------------

QT       += core gui multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = LuaLab
TEMPLATE = app

RC_FILE = app.rc


SOURCES += main.cpp\
        mainwindow.cpp \
    flowlayout.cpp \
    luaengine.cpp \
    Lua/lapi.c \
    Lua/lauxlib.c \
    Lua/lbaselib.c \
    Lua/lbitlib.c \
    Lua/lcode.c \
    Lua/lcorolib.c \
    Lua/lctype.c \
    Lua/ldblib.c \
    Lua/ldebug.c \
    Lua/ldo.c \
    Lua/ldump.c \
    Lua/lfunc.c \
    Lua/lgc.c \
    Lua/linit.c \
    Lua/liolib.c \
    Lua/llex.c \
    Lua/lmathlib.c \
    Lua/lmem.c \
    Lua/loadlib.c \
    Lua/lobject.c \
    Lua/lopcodes.c \
    Lua/loslib.c \
    Lua/lparser.c \
    Lua/lstate.c \
    Lua/lstring.c \
    Lua/lstrlib.c \
    Lua/ltable.c \
    Lua/ltablib.c \
    Lua/ltm.c \
    Lua/lundump.c \
    Lua/lutf8lib.c \
    Lua/lvm.c \
    Lua/lzio.c \
    zlgcan/lzlgcanlib.cpp \
    lguilib.cpp \
    luaevent.cpp \
    luathread.cpp \
    sleeperthread.cpp \
    luamessage/luamessage.cpp \
    luamessage/luamessagewidget.cpp \
    luamessage/luamessagelistmodel.cpp \
    propertyeditor/propertyeditor.cpp \
    propertyeditor/luakvmodel.cpp \
    luamessage/luamessageview.cpp \
    luavalueedit.cpp

HEADERS  += mainwindow.h \
    flowlayout.h \
    luaengine.h \
    Lua/lapi.h \
    Lua/lauxlib.h \
    Lua/lcode.h \
    Lua/lctype.h \
    Lua/ldebug.h \
    Lua/ldo.h \
    Lua/lfunc.h \
    Lua/lgc.h \
    Lua/llex.h \
    Lua/llimits.h \
    Lua/lmem.h \
    Lua/lobject.h \
    Lua/lopcodes.h \
    Lua/lparser.h \
    Lua/lprefix.h \
    Lua/lstate.h \
    Lua/lstring.h \
    Lua/ltable.h \
    Lua/ltm.h \
    Lua/lua.h \
    Lua/lua.hpp \
    Lua/luaconf.h \
    Lua/lualib.h \
    Lua/lundump.h \
    Lua/lvm.h \
    Lua/lzio.h \
    zlgcan/lzlgcanlib.h \
    lguilib.h \
    luaevent.h \
    luathread.h \
    sleeperthread.h \
    luamessage/luamessage.h \
    luamessage/luamessagewidget.h \
    luamessage/luamessagelistmodel.h \
    propertyeditor/propertyeditor.h \
    propertyeditor/luakvmodel.h \
    luamessage/luamessageview.h \
    luavalueedit.h

DISTFILES += \
    config.ini \
    script/script.lua \
    Lua/tasklib.lua \
    Lua/zlgcanlib.lua \
    lualab.ico \
    app.rc \
    edit.cmd \
    tools.txt

LIBS += -L"$$_PRO_FILE_PWD_/zlgcan" -lControlCAN

win32-msvc:QMAKE_CXXFLAGS += /utf-8
