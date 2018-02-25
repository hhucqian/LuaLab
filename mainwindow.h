#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMenu>
#include <QAction>
#include <QMenuBar>
#include <QToolBar>
#include <QStatusBar>
#include <QSettings>
#include <QMainWindow>
#include <QComboBox>
#include <QDockWidget>
#include <QList>
#include <QLabel>
#include <QAction>
#include "luaengine.h"

class LuaValueEdit;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);

protected:
    void closeEvent(QCloseEvent *);
private:
    QSettings m_settings;

    void setupUI();
    void saveConfigFile();
    void updateUI();

    void createFnToolBar();
    int m_fn_count;
    QList<QAction*> m_fn_actions;

    QList<QString> getScriptFileNames();
    QComboBox *m_cbx_scripts;
    bool m_is_pause;
    bool m_is_running;
    void createScriptToolBar();
    QList<QAction*> m_script_tool_actions;
    QAction* m_run_action, *m_paush_action, *m_edit_action;

    void createMenu();
    void createToolsMenu();

    LuaValueEdit *m_value_edit;
    QLabel *m_lbl_script_state;

    QList<QDockWidget*> m_docks;
    QList<QToolBar *> m_tool_bars;

public slots:
    void onLuaEvent(LuaEvent event);
    void onLuaStateChange(bool isRunning);
    void onLuaScriptError(QString msg);
    void onFnClick();
    void onScriptRunClick();
    void onScriptPauseClick();
    void onScriptEditClick();
    void onToolActionClick();
};

#endif // MAINWINDOW_H
