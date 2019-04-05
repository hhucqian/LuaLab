#ifndef LUAEVENTLISTMODEL_H
#define LUAEVENTLISTMODEL_H

#include <QAbstractListModel>
#include <QList>
#include <QObject>
#include <QReadWriteLock>
#include <QTimerEvent>
#include <QWriteLocker>

#include "luamessage.h"

class LuaMessageListModel : public QAbstractListModel
{
    Q_OBJECT
  public:
    explicit LuaMessageListModel(QObject *parent = 0);
    void addMessage(const LuaMessage &msg);

    int rowCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    void setShowTypes(const QList<int> &list);
    void clearDataAndTypes();
    Qt::ItemFlags flags(const QModelIndex &index) const;
    void saveToFile();
    int showMsgCount() const { return this->m_show_msg.size(); };
    int allMsgCount() const { return this->m_all_msg.size(); };

  protected:
    void timerEvent(QTimerEvent *event);

  signals:

  public slots:

  private:
    QList<LuaMessage> m_show_msg;
    QList<LuaMessage> m_all_msg;
    QList<LuaMessage> m_temp_msg;
    QList<int> m_show_types;
    int m_max_row_count;

    void filterShowMsg();

    QReadWriteLock m_rw_lock;
    QReadWriteLock m_temp_lock;
};

#endif // LUAEVENTLISTMODEL_H
