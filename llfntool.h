#ifndef TOOLBARWIDGET_H
#define TOOLBARWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QList>

#include "luaengine.h"

class LLFnTool : public QWidget
{
    Q_OBJECT
public:
    explicit LLFnTool(int n, QWidget *parent = 0);

private:
    int m_fn_count;
    QList<QPushButton*> m_btns;
    void setupUI();

private slots:
    void on_fn_click();

signals:
    void onFnClick(int n);

public slots:
    void onLuaEvent(LuaEvent event);
};

#endif // TOOLBARWIDGET_H
