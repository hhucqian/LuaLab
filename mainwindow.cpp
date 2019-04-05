#include "mainwindow.h"

#include <QApplication>
#include <QDebug>
#include <QDir>
#include <QFileDialog>
#include <QList>
#include <QMessageBox>
#include <QProcess>
#include <QStringList>
#include <QTextEdit>
#include <QToolBar>
#include <QVBoxLayout>

#include "luamessage/luamessagewidget.h"
#include "luavalueedit.h"
#include "propertyeditor/propertyeditor.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      m_settings("config.ini", QSettings::IniFormat, this), m_fn_count(8), m_is_pause(false), m_is_running(false)
{
    this->m_appname = "LuaLab V2.1";
    this->setupUI();
    this->setWindowTitle(this->m_appname);
    this->m_fs_watcher.addPath("script/");
    qRegisterMetaType<LuaEvent>("LuaEvent");
    connect(gL, SIGNAL(GetLuaEvent(LuaEvent)), this, SLOT(onLuaEvent(LuaEvent)));
    connect(gL, SIGNAL(LuaStateChange(QString, bool)), this, SLOT(onLuaStateChange(QString, bool)));
    connect(gL, SIGNAL(LuaScriptError(QString)), this, SLOT(onLuaScriptError(QString)));
    connect(&this->m_fs_watcher, SIGNAL(directoryChanged(QString)), this, SLOT(onScriptChanged()));
}

void MainWindow::setupUI()
{
    QWidget *main_widget = new QWidget();
    QVBoxLayout *main_layout = new QVBoxLayout();
    main_widget->setLayout(main_layout);

    LuaMessageWidget *msgWidget = new LuaMessageWidget();

    main_layout->addWidget(msgWidget, 1);
    main_layout->setMargin(0);
    this->setCentralWidget(main_widget);

    PropertyEditor *peditor = new PropertyEditor();

    QDockWidget *dock1 = new QDockWidget(QString::fromUtf8("键值显示器"));
    dock1->setObjectName("dock_peditor");
    dock1->setWidget(peditor);
    this->m_value_edit = new LuaValueEdit();
    QDockWidget *dock2 = new QDockWidget(QString::fromUtf8("值编辑器"));
    dock2->setObjectName("dock_veditor");
    dock2->setWidget(this->m_value_edit);

    this->m_docks.append(dock1);
    this->m_docks.append(dock2);
    this->addDockWidget(Qt::RightDockWidgetArea, dock1);
    this->addDockWidget(Qt::RightDockWidgetArea, dock2);

    this->createScriptToolBar();
    this->createFnToolBar();
    this->createMenu();

    this->m_lbl_script_state = new QLabel(QString::fromUtf8("未运行"));
    this->statusBar()->addWidget(this->m_lbl_script_state);

    this->restoreGeometry(this->m_settings.value("gui/main").toByteArray());
    this->restoreState(this->m_settings.value("gui/state").toByteArray());
    for (int i = 0; i < this->m_docks.size(); ++i)
    {
        this->restoreDockWidget(this->m_docks.at(i));
    }
    int index = this->m_settings.value("gui/script", 0).toInt();
    if (index > this->m_cbx_scripts->count())
    {
        index = 0;
    }
    this->m_cbx_scripts->setCurrentIndex(index);
    this->m_value_edit->restoreConfig(this->m_settings);
}

void MainWindow::closeEvent(QCloseEvent *)
{
    this->saveConfigFile();
}

void MainWindow::saveConfigFile()
{
    this->m_settings.setValue("gui/main", this->saveGeometry());
    this->m_settings.setValue("gui/state", this->saveState());
    this->m_settings.setValue("gui/script", this->m_cbx_scripts->currentIndex());
    this->m_value_edit->saveConfig(this->m_settings);
}

void MainWindow::onLuaEvent(LuaEvent event)
{
    switch (event.Type)
    {
    case LuaEvent::EVENT_SET_TITLE:
        this->setWindowTitle(QString("%1 - %2").arg(this->m_appname, event.ExtraString));
        break;
    case LuaEvent::EVENT_SET_FN_TEXT:
        this->m_fn_actions.at(event.ExtraInt - 1)->setText(event.ExtraString);
        break;
    default:
        break;
    }
}

void MainWindow::onLuaScriptError(QString msg)
{
    QMessageBox::warning(this, QString::fromUtf8("脚本错误"), msg);
}

void MainWindow::onLuaStateChange(QString, bool isRunning)
{
    this->m_is_running = isRunning;
    this->m_is_pause = false;
    this->updateUI();
    for (int i = 0; i < this->m_fn_count; ++i)
    {
        this->m_fn_actions.at(i)->setText(QString("F%1").arg(i + 1));
    }
}

void MainWindow::updateUI()
{
    if (this->m_is_running)
    {
        this->m_lbl_script_state->setText(QString::fromUtf8("<font color='green'>运行中</font>"));
        this->m_run_action->setText(QString::fromUtf8("停止"));
    }
    else
    {
        this->m_lbl_script_state->setText(QString::fromUtf8("未运行"));
        this->m_run_action->setText(QString::fromUtf8("运行"));
    }
    if (this->m_is_pause)
    {
        this->m_paush_action->setText(QString::fromUtf8("继续"));
    }
    else
    {
        this->m_paush_action->setText(QString::fromUtf8("暂停"));
    }
}

void MainWindow::createFnToolBar()
{
    for (int i = 0; i < this->m_fn_count; ++i)
    {
        QAction *item = new QAction(QString("F%1").arg(i + 1), this);
        this->m_fn_actions.append(item);
        connect(item, SIGNAL(triggered(bool)), this, SLOT(onFnClick()));
    }
    QToolBar *toolBar = new QToolBar();
    toolBar->addActions(this->m_fn_actions);
    toolBar->setObjectName("fn_tool_bar");
    toolBar->setWindowTitle(QString::fromUtf8("Fn 工具"));
    this->addToolBar(Qt::TopToolBarArea, toolBar);
    this->m_tool_bars.append(toolBar);
}

void MainWindow::onFnClick()
{
    for (int i = 0; i < this->m_fn_count; ++i)
    {
        if (sender() == this->m_fn_actions.at(i))
        {
            LuaEvent event;
            event.Type = LuaEvent::EVENT_FUN;
            event.ExtraType = LuaEvent::EXTRA_INT;
            event.ExtraInt = i + 1;
            gL->putEvent(event);
        }
    }
}

QList<QString> MainWindow::getScriptFileNames()
{
    QList<QString> res;
    QDir dir("script");
    QStringList files = dir.entryList(QDir::Files);
    QString filename;
    foreach (filename, files)
    {
        if (filename.endsWith(".lua"))
        {
            res.append(filename);
        }
    }
    return res;
}

void MainWindow::createScriptToolBar()
{
    this->m_cbx_scripts = new QComboBox();
    this->m_cbx_scripts->addItems(this->getScriptFileNames());
    QAction *action = new QAction(QString::fromUtf8("运行"), this);
    this->m_script_tool_actions.append(action);
    this->m_run_action = action;
    action = new QAction(QString::fromUtf8("暂停"), this);
    this->m_script_tool_actions.append(action);
    this->m_paush_action = action;
    action = new QAction(QString::fromUtf8("编辑"), this);
    this->m_script_tool_actions.append(action);
    this->m_edit_action = action;
    action = new QAction(QString::fromUtf8("消音"), this);
    this->m_script_tool_actions.append(action);
    this->m_stop_media_action = action;
    QToolBar *toolBar = new QToolBar();
    toolBar->addWidget(this->m_cbx_scripts);
    toolBar->addActions(this->m_script_tool_actions);
    toolBar->setObjectName("script_tool_bar");
    toolBar->setWindowTitle(QString::fromUtf8("脚本工具"));
    this->addToolBar(Qt::TopToolBarArea, toolBar);
    this->m_tool_bars.append(toolBar);
    connect(this->m_run_action, SIGNAL(triggered(bool)), this, SLOT(onScriptRunClick()));
    connect(this->m_paush_action, SIGNAL(triggered(bool)), this, SLOT(onScriptPauseClick()));
    connect(this->m_edit_action, SIGNAL(triggered(bool)), this, SLOT(onScriptEditClick()));
    connect(this->m_stop_media_action, SIGNAL(triggered(bool)), this, SLOT(onStopMediaClick()));
}

void MainWindow::onScriptRunClick()
{
    if (gL->isRunning())
    {
        gL->stopScript();
    }
    else
    {
        gL->runLuaThread("script/" + this->m_cbx_scripts->currentText());
    }
}

void MainWindow::onScriptPauseClick()
{
    if (gL->isRunning())
    {
        if (this->m_is_pause)
        {
            gL->resumeScript();
            this->m_is_pause = false;
        }
        else
        {
            gL->pauseScript();
            this->m_is_pause = true;
        }
        this->updateUI();
    }
}

void MainWindow::onScriptEditClick()
{
    QString stript_file = QDir::currentPath() + "/script/" + this->m_cbx_scripts->currentText();
    QProcess::execute("edit.cmd", QStringList() << QDir::currentPath() + "/script/" << stript_file);
}

void MainWindow::createMenu()
{
    QMenu *menu = NULL;
    QAction *action = NULL;
    menu = this->menuBar()->addMenu(QString::fromUtf8("文件 (&F)"));
    menu->addActions(this->m_script_tool_actions);
    menu->addSeparator();
    action = menu->addAction(QString::fromUtf8("大"));
    connect(action, SIGNAL(triggered(bool)), this, SLOT(onFontBigClick()));
    action = menu->addAction(QString::fromUtf8("中"));
    connect(action, SIGNAL(triggered(bool)), this, SLOT(onFontMidClick()));
    action = menu->addAction(QString::fromUtf8("正常"));
    connect(action, SIGNAL(triggered(bool)), this, SLOT(onFontNormalClick()));
    menu->addSeparator();
    action = menu->addAction(QString::fromUtf8("退出 (&X)"));
    connect(action, SIGNAL(triggered(bool)), this, SLOT(close()));
    menu = this->menuBar()->addMenu(QString::fromUtf8("Fn 信号"));
    menu->addActions(this->m_fn_actions);
    menu = this->menuBar()->addMenu(QString::fromUtf8("视图 (&V)"));
    for (int i = 0; i < this->m_docks.size(); ++i)
    {
        menu->addAction(this->m_docks.at(i)->toggleViewAction());
    }
    menu->addSeparator();
    for (int i = 0; i < this->m_tool_bars.size(); ++i)
    {
        menu->addAction(this->m_tool_bars.at(i)->toggleViewAction());
    }
    if (QFile::exists("tools.txt"))
    {
        this->createToolsMenu();
    }
}

void MainWindow::createToolsMenu()
{
    QMenu *menu = this->menuBar()->addMenu(QString::fromUtf8("工具 (&T)"));
    QAction *action = NULL;
    QFile file("tools.txt");
    file.open(QFile::ReadOnly);
    QString file_content = QString::fromUtf8(file.readAll());
    file.close();
    QStringList lines = file_content.split("\n", QString::SkipEmptyParts);
    int index = 1;
    for (int i = 0; i < lines.size(); ++i)
    {
        QStringList parts = lines.at(i).split("#", QString::SkipEmptyParts);
        if (parts.length() > 1)
        {
            if (index < 10)
            {
                action = menu->addAction(QString("%1 (&%2)").arg(parts[0]).arg(index));
            }
            else if ('Z' - 'A' - index + 10 >= 0)
            {
                action = menu->addAction(QString("%1 (&%2)").arg(parts[0]).arg((char)('A' + index - 10)));
            }
            else
            {
                action = menu->addAction(QString("%1").arg(parts[0]));
            }
            action->setData(parts[1]);
            connect(action, SIGNAL(triggered(bool)), this, SLOT(onToolActionClick()));
            ++index;
        }
    }
}

void MainWindow::onToolActionClick()
{
    QString cmd = ((QAction *)sender())->data().toString();
    QProcess::startDetached(cmd);
}

void MainWindow::onScriptChanged()
{
    this->m_cbx_scripts->clear();
    this->m_cbx_scripts->addItems(this->getScriptFileNames());
}

void MainWindow::onFontBigClick()
{
    qApp->setStyleSheet("*{font-size : 30px;}");
}

void MainWindow::onFontMidClick()
{
    qApp->setStyleSheet("*{font-size : 20px;}");
}

void MainWindow::onFontNormalClick()
{
    qApp->setStyleSheet("");
}

void MainWindow::onStopMediaClick()
{
    gL->triggerStopMedia();
}
