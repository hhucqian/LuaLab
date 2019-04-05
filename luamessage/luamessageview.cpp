#include "luamessageview.h"
#include <QApplication>
#include <QClipboard>

LuaMessageView::LuaMessageView(QWidget *parent) : QListView(parent)
{
    setFont(QFont("Consolas, 'Courier New', monospace"));
}

void LuaMessageView::keyPressEvent(QKeyEvent *event)
{
    if (event->matches(QKeySequence::Copy))
    {
        int itemsCount = this->selectedIndexes().size();
        QStringList strings;
        for (int i = 0; i < itemsCount; ++i)
        {
            strings << this->model()->data(this->selectedIndexes().at(i)).toString();
        }

        QApplication::clipboard()->setText(strings.join("\n"));
    }
}
