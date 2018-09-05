#include "propertyeditor.h"
#include "luakvmodel.h"
#include "../luaengine.h"
#include <QHBoxLayout>
#include <QHeaderView>
#include <QTableWidget>
#include <QAbstractItemView>

PropertyEditor::PropertyEditor(QWidget *parent) : QWidget(parent)
{
    this->setupUI();
    connect(gL, SIGNAL(SetKVEvent(QString,QString,QString)), this, SLOT(onSetKV(QString,QString,QString)));
    connect(gL, SIGNAL(LuaStateChange(QString, bool)), this, SLOT(onLuaStateChange(QString, bool)));
}

void PropertyEditor::setupUI()
{
    this->m_model = new LuaKVModel();
    this->m_table_view = new QTableView();
    this->m_table_view->setModel(this->m_model);
    this->m_table_view->verticalHeader()->setVisible(false);
    this->m_table_view->horizontalHeader()->setStretchLastSection(true);
    this->m_table_view->setAlternatingRowColors(true);
    this->m_table_view->setSelectionBehavior(QTableView::SelectRows);
    this->m_table_view->setEditTriggers(QTableView::DoubleClicked);

    QHBoxLayout *main_layout = new QHBoxLayout();
    main_layout->addWidget(this->m_table_view);
    main_layout->setMargin(0);

    this->setLayout(main_layout);
}

void PropertyEditor::onSetKV(QString key, QString value, QString color)
{
    this->m_model->setKV(key, value, color);
}

void PropertyEditor::onLuaStateChange(QString, bool isRunning)
{
    if(isRunning)
    {
        this->m_model->clearKV();
    }
}
