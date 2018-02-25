#include "luamessagelistmodel.h"
#include <QBrush>
#include <QColor>

LuaMessageListModel::LuaMessageListModel(QObject *parent) : QAbstractListModel(parent),m_max_row_count(500), m_need_reset(false)
{
    this->startTimer(50);
}

void LuaMessageListModel::addMessage(const LuaMessage &msg)
{
    QWriteLocker locker(&this->m_rw_lock);
    this->m_all_msg.insert(0, msg);
    if(this->m_show_types.size() == 0 || this->m_show_types.contains(msg.Type())) {
        this->m_show_msg.insert(0, &this->m_all_msg.first());
        QWriteLocker reset_lock(&this->m_reset_lock);
        this->m_need_reset = true;
    }
}

void LuaMessageListModel::timerEvent(QTimerEvent */*event*/)
{
    QWriteLocker reset_lock(&this->m_reset_lock);
    if(this->m_need_reset) {
        this->m_need_reset = false;
        this->reset();
    }
}


int LuaMessageListModel::rowCount(const QModelIndex &/*parent*/) const
{
    QReadLocker locker(const_cast<QReadWriteLock*>(&this->m_rw_lock));
    return this->m_show_msg.size() > this->m_max_row_count ? this->m_max_row_count : this->m_show_msg.size();
}

QVariant LuaMessageListModel::data(const QModelIndex &index, int role) const
{
    QReadLocker locker(const_cast<QReadWriteLock*>(&this->m_rw_lock));
    if(role == Qt::DisplayRole || role == Qt::EditRole) {
        return this->m_show_msg.at(index.row())->Msg();
    }
    return QVariant();
}

void LuaMessageListModel::setShowTypes(const QList<int> &list)
{
    this->m_show_types.clear();
    this->m_show_types.append(list);
    this->filterShowMsg();
}

void LuaMessageListModel::filterShowMsg()
{
    QWriteLocker locker(&this->m_rw_lock);
    this->m_show_msg.clear();
    for(QList<LuaMessage>::iterator it = this->m_all_msg.begin(); it != this->m_all_msg.end(); ++it)
    {
        if(this->m_show_types.size() == 0 || this->m_show_types.contains((*it).Type()))
        {
            this->m_show_msg.append(&(*it));
            if(this->m_show_msg.count() >= this->m_max_row_count)
            {
                break;
            }
        }
    }
    this->reset();
}

void LuaMessageListModel::clearDataAndTypes()
{
    this->m_show_msg.clear();
    this->m_all_msg.clear();
    this->m_show_types.clear();
    this->reset();
}

Qt::ItemFlags LuaMessageListModel::flags(const QModelIndex &index) const
{
    return QAbstractListModel::flags(index) | Qt::ItemIsEditable;
}
