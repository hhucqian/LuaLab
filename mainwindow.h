#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMenu>
#include <QAction>
#include <QMenuBar>
#include <QToolBar>
#include <QStatusBar>
#include <QSettings>
#include <QMainWindow>
#include <QDockWidget>
#include <QList>
#include <QLabel>
#include <QAction>
#include "luaengine.h"

class LLScriptTool;
class LuaValueEdit;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent *);
private:
    QSettings m_settings;

    void setupUI();
    void saveConfigFile();
    void createActions();

    void createFnToolBar();
    int m_fn_count;
    QList<QAction*> m_fn_actions;

    LLScriptTool *m_script_tool;
    LuaValueEdit *m_value_edit;
    QLabel *m_lbl_script_state;

    QList<QDockWidget*> m_docks;

public slots:
    void onLuaEvent(LuaEvent event);
    void onLuaStateChange(bool isRunning);
    void onLuaScriptError(QString msg);
    void onFnClick();
};

#endif // MAINWINDOW_H
