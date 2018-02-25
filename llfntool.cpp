#include "llfntool.h"
#include "flowlayout.h"
#include <QStyleOption>
#include <QPainter>
#include <QHBoxLayout>


LLFnTool::LLFnTool(int n, QWidget *parent) : QWidget(parent), m_fn_count(n)
{
    this->setupUI();
    connect(gL, SIGNAL(GetLuaEvent(LuaEvent)), this, SLOT(onLuaEvent(LuaEvent)));
}

void LLFnTool::setupUI()
{
    QHBoxLayout *main_layout = new QHBoxLayout();
    for(int i = 0; i < this->m_fn_count; ++i) {
        QPushButton *btn = new QPushButton(QString::fromUtf8("Fn %1").arg(i + 1));
        this->m_btns.append(btn);
        main_layout->addWidget(btn);
        connect(btn, SIGNAL(clicked(bool)), this, SLOT(on_fn_click()));
    }
    main_layout->setMargin(0);
    this->setLayout(main_layout);
}

void LLFnTool::on_fn_click()
{
    for(int i = 0; i < this->m_fn_count; ++i) {
        if(this->m_btns.at(i) == sender()) {
            LuaEvent event;
            event.Type = LuaEvent::EVENT_FUN;
            event.ExtraType = LuaEvent::EXTRA_INT;
            event.ExtraInt = i + 1;
            gL->putEvent(event);
            emit onFnClick(i+1);
        }
    }
}

void LLFnTool::onLuaEvent(LuaEvent event)
{
    if(event.Type == LuaEvent::EVENT_SET_FN) {
        if(event.ExtraInt > 0 && event.ExtraInt <= this->m_btns.size()) {
            this->m_btns[event.ExtraInt - 1]->setText(event.ExtraString);
        }
    }
}
