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
#include "luaengine.h"

class LLFnTool;
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

    LLFnTool *m_fn_tool;
    LLScriptTool *m_script_tool;
    LuaValueEdit *m_value_edit;
    QLabel *m_lbl_script_state;

    QList<QDockWidget*> m_docks;

public slots:
    void onLuaEvent(LuaEvent event);
    void onLuaStateChange(bool isRunning);
    void onLuaScriptError(QString msg);
};

#endif // MAINWINDOW_H
