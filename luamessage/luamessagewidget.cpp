#include "luamessagewidget.h"
#include <QDebug>
#include <QListView>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include "../luaengine.h"
#include "../flowlayout.h"
#include "luamessageview.h"

LuaMessageWidget::LuaMessageWidget(QWidget *parent) : QWidget(parent)
{
    this->m_msg_types.append(QString::fromUtf8("所有"));
    this->setupUI();
}

void LuaMessageWidget::setupUI()
{

    QVBoxLayout *main_layout = new QVBoxLayout();

    QHBoxLayout *tool_layout = new QHBoxLayout();
    FlowLayout *type_layout = new FlowLayout();
    QCheckBox *cbx = new QCheckBox();
    cbx->setText(this->m_msg_types.at(0));
    cbx->setCheckState(Qt::Checked);
    this->m_type_cbx.append(cbx);
    type_layout->addWidget(cbx);

    this->m_list_view = new LuaMessageView();
    this->m_list_view->setModel(&this->m_model);
    this->m_list_view->setEditTriggers(QListView::DoubleClicked);
    this->m_list_view->setAlternatingRowColors(true);
    this->m_list_view->setSelectionMode(QAbstractItemView::ExtendedSelection);

    QPushButton *btn_save = new QPushButton();
    btn_save->setText(QString::fromUtf8("保存"));

    tool_layout->addLayout(type_layout, 2);
    tool_layout->addStretch(1);
    tool_layout->addWidget(btn_save);
    main_layout->addLayout(tool_layout);
    main_layout->addWidget(this->m_list_view);
    this->setLayout(main_layout);
    this->m_type_layout = type_layout;

    connect(gL, SIGNAL(PushMsg(int,QString)), this, SLOT(onGetMsg(int,QString)));
    connect(gL, SIGNAL(AddMsgType(QString)), this, SLOT(onAddMsgType(QString)));
    connect(gL, SIGNAL(LuaStateChange(QString, bool)), this, SLOT(onLuaStateChange(QString, bool)));
    connect(cbx, SIGNAL(clicked(bool)), this, SLOT(onCbxClick(bool)));
    connect(btn_save, SIGNAL(clicked(bool)), this, SLOT(onSaveClick()));
}

void LuaMessageWidget::onGetMsg(int type, QString msg)
{
    this->m_model.addMessage(LuaMessage(type, msg));
}

void LuaMessageWidget::onAddMsgType(QString name)
{
    if(!this->m_msg_types.contains(name)) {
        this->m_msg_types.append(name);
        QCheckBox *cbx = new QCheckBox();
        cbx->setText(name);
        cbx->setCheckState(Qt::Checked);
        this->m_type_cbx.append(cbx);
        this->m_type_layout->addWidget(cbx);
        this->m_model.setShowTypes(this->getShowTypes());
        connect(cbx, SIGNAL(clicked(bool)), this, SLOT(onCbxClick(bool)));

        for(int i = 0; i < this->m_msg_types.size() && i < this->m_check_state.value(this->m_current_script_id)->size(); ++i) {
            this->m_type_cbx.at(i)->setChecked(this->m_check_state.value(this->m_current_script_id)->at(i));
        }
    }
}

void LuaMessageWidget::onCbxClick(bool checked)
{
    if(sender() == this->m_type_cbx.at(0)) {
        for(int i = 1; i < this->m_type_cbx.size(); ++i) {
            this->m_type_cbx.at(i)->setChecked(checked);
        }
    } else {
        int checkcount = 0;
        for(int i = 1; i < this->m_type_cbx.size(); ++i) {
            if(this->m_type_cbx.at(i)->isChecked()) {
                ++checkcount;
            }
        }
        if(checkcount == 0) {
            this->m_type_cbx.at(0)->setCheckState(Qt::Unchecked);
        } else if(checkcount == this->m_type_cbx.size() - 1) {
            this->m_type_cbx.at(0)->setCheckState(Qt::Checked);
        } else {
            this->m_type_cbx.at(0)->setCheckState(Qt::PartiallyChecked);
        }
    }

    this->m_model.setShowTypes(this->getShowTypes());
}

QList<int> LuaMessageWidget::getShowTypes()
{
    QList<int> res;
    for(int i = 1; i < this->m_type_cbx.size(); ++i) {
        if(this->m_type_cbx.at(i)->isChecked()) {
            res.append(i);
        }
    }
    if(res.size() == this->m_type_cbx.size() - 1) {
        res.clear();
    }
    return res;
}

void LuaMessageWidget::onLuaStateChange(QString id, bool isRunning)
{
    this->m_current_script_id = id;
    if(!this->m_check_state.contains(id)) {
        this->m_check_state.insert(id, new QList<bool>());
    }
    if(isRunning) {
        this->m_model.clearDataAndTypes();
        while(this->m_msg_types.size() > 1) {
            this->m_msg_types.removeLast();
        }
        while(this->m_type_cbx.size() > 1) {
            this->m_type_layout->removeWidget(this->m_type_cbx.last());
            this->m_type_cbx.last()->deleteLater();
            this->m_type_cbx.removeLast();
        }
    } else {
        this->m_check_state.value(this->m_current_script_id)->clear();
        for(int i = 0; i < this->m_type_cbx.size(); ++i) {
            this->m_check_state.value(this->m_current_script_id)->append(this->m_type_cbx[i]->isChecked());
        }
    }
}

void LuaMessageWidget::onSaveClick()
{
    this->m_model.saveToFile();
}
