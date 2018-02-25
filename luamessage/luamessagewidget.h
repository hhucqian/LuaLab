#ifndef LUAMESSAGEWIDGET_H
#define LUAMESSAGEWIDGET_H

#include <QWidget>
#include <QList>
#include <QString>
#include <QCheckBox>
#include <QLayout>
#include <QListView>
#include "luamessagelistmodel.h"

class LuaMessageWidget : public QWidget
{
    Q_OBJECT
public:
    explicit LuaMessageWidget(QWidget *parent = 0);

signals:

private:
    void setupUI();

    LuaMessageListModel m_model;
    QList<QString> m_msg_types;
    QList<QCheckBox*> m_type_cbx;

    QLayout *m_type_layout;
    QListView *m_list_view;

    QList<int> getShowTypes();

public slots:
    void onGetMsg(int type, QString msg);
    void onAddMsgType(QString name);
    void onCbxClick(bool checked);
    void onLuaStateChange(bool isRunning);
};

#endif // LUAMESSAGEWIDGET_H
