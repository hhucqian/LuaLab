#ifndef LUAMESSAGEVIEW_H
#define LUAMESSAGEVIEW_H

#include <QKeyEvent>
#include <QListView>

class LuaMessageView : public QListView
{
    Q_OBJECT
  public:
    explicit LuaMessageView(QWidget *parent = 0);

  protected:
    void keyPressEvent(QKeyEvent *event);

  signals:

  public slots:
};

#endif // LUAMESSAGEVIEW_H
