#include "mainwindow.h"

#include <QList>
#include <QDebug>
#include <QMessageBox>
#include <QFileDialog>
#include <QVBoxLayout>
#include <QToolBar>
#include <QMessageBox>
#include <QTextEdit>
#include <QComboBox>


#include "llfntool.h"
#include "llscripttool.h"
#include "luavalueedit.h"
#include "luamessage/luamessagewidget.h"
#include "propertyeditor/propertyeditor.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      m_settings("config.ini", QSettings::IniFormat, this)
{
    this->setupUI();
    this->setWindowTitle(QString::fromUtf8("LuaLab V2.0"));
    qRegisterMetaType<LuaEvent>("LuaEvent");
    connect(gL, SIGNAL(GetLuaEvent(LuaEvent)), this, SLOT(onLuaEvent(LuaEvent)));
    connect(gL, SIGNAL(LuaStateChange(bool)), this, SLOT(onLuaStateChange(bool)));
    connect(gL, SIGNAL(LuaScriptError(QString)), this, SLOT(onLuaScriptError(QString)));
}

MainWindow::~MainWindow()
{

}

void MainWindow::setupUI()
{
    this->m_fn_tool = new LLFnTool(8);
    this->m_script_tool = new LLScriptTool();

    QWidget *main_widget = new QWidget();
    QVBoxLayout *main_layout = new QVBoxLayout();
    main_widget->setLayout(main_layout);

    QToolBar *script_tool_bar = new QToolBar();
    script_tool_bar->addWidget(this->m_script_tool);
    script_tool_bar->setObjectName("script_tool_bar");
    script_tool_bar->setAllowedAreas(Qt::BottomToolBarArea | Qt::TopToolBarArea);
    this->addToolBar(Qt::TopToolBarArea, script_tool_bar);
    QToolBar *fn_tool_bar = new QToolBar();
    fn_tool_bar->addWidget(this->m_fn_tool);
    fn_tool_bar->setObjectName("fn_tool_bar");
    fn_tool_bar->setAllowedAreas(Qt::BottomToolBarArea | Qt::TopToolBarArea);
    this->insertToolBar(script_tool_bar, fn_tool_bar);

    LuaMessageWidget *msgWidget = new LuaMessageWidget();

    main_layout->addWidget(msgWidget, 1);
    main_layout->setMargin(0);
    this->setCentralWidget(main_widget);

    PropertyEditor *peditor = new PropertyEditor();

    QDockWidget *dock1 = new QDockWidget(QString::fromUtf8("键值显示器"));
    dock1->setObjectName("dock_peditor");
    dock1->setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable);
    dock1->setWidget(peditor);
    this->m_value_edit = new LuaValueEdit();
    QDockWidget *dock2 = new QDockWidget(QString::fromUtf8("值编辑器"));
    dock2->setObjectName("dock_veditor");
    dock2->setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable);
    dock2->setWidget(this->m_value_edit);

    this->addDockWidget(Qt::RightDockWidgetArea, dock1);
    this->addDockWidget(Qt::RightDockWidgetArea, dock2);

    this->m_lbl_script_state = new QLabel(QString::fromUtf8("未运行"));
    this->statusBar()->addWidget(this->m_lbl_script_state);

    this->restoreGeometry(this->m_settings.value("gui/main").toByteArray());
    this->restoreState(this->m_settings.value("gui/state").toByteArray());
    for(int i = 0; i < this->m_docks.size(); ++i) {
        this->restoreDockWidget(this->m_docks.at(i));
    }
    this->m_script_tool->restoreConfig(this->m_settings);
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
    this->m_script_tool->saveConfig(this->m_settings);
    this->m_value_edit->saveConfig(this->m_settings);
}

void MainWindow::onLuaEvent(LuaEvent event)
{
    if(event.Type == LuaEvent::EVENT_SET_TITLE) {
        this->setWindowTitle(QString("LuaLab V2.0 - %1").arg(event.ExtraString));
    }
}

void MainWindow::onLuaScriptError(QString msg)
{
   QMessageBox::warning(this, QString::fromUtf8("脚本错误"), msg);
}

void MainWindow::onLuaStateChange(bool isRunning)
{
    if(isRunning) {
        this->m_lbl_script_state->setText(QString::fromUtf8("<font color='green'>运行中</font>"));
    } else {
        this->m_lbl_script_state->setText(QString::fromUtf8("未运行"));
    }
}
