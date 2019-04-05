#include "luavalueedit.h"
#include "luaengine.h"
#include <QHBoxLayout>
#include <QHash>
#include <QPushButton>
#include <QVBoxLayout>

LuaValueEdit::LuaValueEdit(QWidget *parent) : QWidget(parent)
{
    this->setupUI();
    connect(gL, SIGNAL(LuaStateChange(QString, bool)), this, SLOT(onLuaStateChange(QString, bool)));
}

void LuaValueEdit::setupUI()
{
    QVBoxLayout *main_layout = new QVBoxLayout();

    this->m_editor = new QTextEdit();
    main_layout->addWidget(this->m_editor);
    main_layout->setMargin(0);

    this->setLayout(main_layout);
    connect(this->m_editor, SIGNAL(textChanged()), this, SLOT(onTextChangedEvent()));
}

void LuaValueEdit::saveConfig(QSettings &settings)
{
    settings.setValue("gui/content", this->m_editor->toPlainText());
}

void LuaValueEdit::restoreConfig(QSettings &settings)
{
    this->m_editor->setPlainText(settings.value("gui/content", "").toString());
}

void LuaValueEdit::onTextChangedEvent()
{
    LuaEvent event;
    event.Type = LuaEvent::EVENT_SET_TEXT;
    event.ExtraType = LuaEvent::EXTRA_HASH;
    QHash<QString, QString> res;
    QString content = this->m_editor->toPlainText();
    QStringList lines = content.split('\n', QString::SkipEmptyParts);
    for (int i = 0; i < lines.size(); ++i)
    {
        QString line = lines.at(i);
        if (line.contains('='))
        {
            QString key = line.left(line.indexOf('='));
            QString value = line.mid(line.indexOf('=') + 1);
            res.insert(key, value);
        }
        else
        {
            res.insert(line, line);
        }
    }
    event.ExtraHash = res;
    gL->putEvent(event);
}

void LuaValueEdit::onLuaStateChange(QString, bool isRunning)
{
    if (isRunning)
    {
        this->onTextChangedEvent();
    }
}
