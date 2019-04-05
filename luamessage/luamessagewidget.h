#ifndef LUAMESSAGEWIDGET_H
#define LUAMESSAGEWIDGET_H

#include "luamessagelistmodel.h"
#include <QCheckBox>
#include <QLayout>
#include <QList>
#include <QListView>
#include <QString>
#include <QVector>
#include <QWidget>

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
    QList<QCheckBox *> m_type_cbx;

    QLayout *m_type_layout;
    QListView *m_list_view;

    QList<int> getShowTypes();
    QHash<QString, QList<bool> *> m_check_state;
    QString m_current_script_id;

  public slots:
    void onGetMsg(int type, QString msg);
    void onAddMsgType(QString name);
    void onCbxClick(bool checked);
    void onLuaStateChange(QString id, bool isRunning);
    void onSaveClick();
};

#endif // LUAMESSAGEWIDGET_H
