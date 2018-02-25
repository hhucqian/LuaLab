#include "llscripttool.h"
#include <QHBoxLayout>
#include <QDir>
#include <QStringList>
#include <QProcess>
#include "luaengine.h"

LLScriptTool::LLScriptTool(QWidget *parent) : QWidget(parent)
{
    this->m_is_pause = false;
    this->setupUI();
    this->updateUI();
}

void LLScriptTool::setupUI()
{
    this->m_btn_edit = new QPushButton(QString::fromUtf8("编辑"));
    this->m_btn_run = new QPushButton(QString::fromUtf8("运行"));
    this->m_btn_pause = new QPushButton(QString::fromUtf8("暂停"));
    this->m_cbx_scripts = new QComboBox();

    QVector<QString> scripts = this->getScriptFileNames();
    this->m_cbx_scripts->addItems(scripts.toList());

    QHBoxLayout *main_layout = new QHBoxLayout();
    main_layout->addWidget(this->m_cbx_scripts);
    main_layout->addWidget(this->m_btn_run);
    main_layout->addWidget(this->m_btn_pause);
    main_layout->addWidget(this->m_btn_edit);

    main_layout->setMargin(0);
    this->setLayout(main_layout);

    connect(this->m_btn_run, SIGNAL(clicked(bool)), this, SLOT(onRunClick()));
    connect(this->m_btn_pause, SIGNAL(clicked(bool)), this, SLOT(onPauseClick()));
    connect(this->m_btn_edit, SIGNAL(clicked(bool)), this, SLOT(onEditClick()));
    connect(gL, SIGNAL(LuaStateChange(bool)), this, SLOT(onLuaStateChange(bool)));
}

void LLScriptTool::updateUI()
{
    if(gL->isRunning()) {
        this->m_btn_run->setText(QString::fromUtf8("停止"));
    } else {
        this->m_btn_run->setText(QString::fromUtf8("运行"));
    }
    if(this->m_is_pause) {
        this->m_btn_pause->setText(QString::fromUtf8("继续"));
    } else {
        this->m_btn_pause->setText(QString::fromUtf8("暂停"));
    }
}

QVector<QString> LLScriptTool::getScriptFileNames()
{
    QVector<QString> res;
    QDir dir("script");
    QStringList files = dir.entryList(QDir::Files);
    QString filename;
    foreach (filename, files) {
        if(filename.endsWith(".lua")) {
            res.append(filename);
        }
    }
    return res;
}

void LLScriptTool::saveConfig(QSettings &settings)
{
    settings.setValue("gui/script", this->m_cbx_scripts->currentIndex());
}

void LLScriptTool::restoreConfig(QSettings &settings)
{
    int index = settings.value("gui/script", 0).toInt();
    if(index > this->m_cbx_scripts->count()) {
        index = 0;
    }
    this->m_cbx_scripts->setCurrentIndex(index);
}

void LLScriptTool::onRunClick()
{
    if(gL->isRunning()) {
        gL->stopScript();
    } else {
        gL->runLuaThread("script/" + this->m_cbx_scripts->currentText());
    }
}

void LLScriptTool::onPauseClick()
{
    if(gL->isRunning()) {
        if(this->m_is_pause) {
            gL->resumeScript();
            this->m_is_pause = false;
        } else {
            gL->pauseScript();
            this->m_is_pause = true;
        }
        this->updateUI();
    }
}

void LLScriptTool::onLuaStateChange(bool /*isRunning*/)
{
    this->m_is_pause = false;
    this->updateUI();
}

void LLScriptTool::onEditClick()
{
    QString stript_file = QDir::currentPath() + "/script/" + this->m_cbx_scripts->currentText();
    QProcess::execute("edit.cmd", QStringList() << QDir::currentPath() + "/script/" << stript_file);
}
