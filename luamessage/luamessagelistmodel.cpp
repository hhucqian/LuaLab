#include "luamessagelistmodel.h"
#include <QBrush>
#include <QColor>
#include <QFile>
#include <QFileDialog>

LuaMessageListModel::LuaMessageListModel(QObject *parent) : QAbstractListModel(parent), m_max_row_count(1000)
{
    this->startTimer(200);
}

void LuaMessageListModel::addMessage(const LuaMessage &msg)
{
    QWriteLocker temp_locker(&this->m_temp_lock);
    this->m_temp_msg.append(msg);
}

void LuaMessageListModel::timerEvent(QTimerEvent *)
{
    QWriteLocker temp_locker(&this->m_temp_lock);
    QWriteLocker locker(const_cast<QReadWriteLock *>(&this->m_rw_lock));

    if (!this->m_temp_msg.empty())
    {
        this->beginResetModel();
        foreach (const auto &msg, this->m_temp_msg)
        {
            this->m_all_msg.insert(0, msg);
            if (this->m_show_types.empty() || this->m_show_types.contains(msg.Type()))
            {
                this->m_show_msg.insert(0, msg);
            }
        }
        this->m_temp_msg.clear();
        while (!this->m_all_msg.empty() && this->m_all_msg.size() > 10 * this->m_max_row_count && this->m_all_msg.last().isExpire(5 * 60))
        {
            this->m_all_msg.removeLast();
        }
        while (!this->m_show_msg.empty() && this->m_show_msg.size() > this->m_max_row_count)
        {
            this->m_show_msg.removeLast();
        }

        this->endResetModel();
    }
}

int LuaMessageListModel::rowCount(const QModelIndex & /*parent*/) const
{
    QReadLocker locker(const_cast<QReadWriteLock *>(&this->m_rw_lock));
    return this->m_show_msg.size();
}

QVariant LuaMessageListModel::data(const QModelIndex &index, int role) const
{
    QReadLocker locker(const_cast<QReadWriteLock *>(&this->m_rw_lock));
    if (role == Qt::DisplayRole || role == Qt::EditRole)
    {
        return this->m_show_msg.at(index.row()).Msg();
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
    this->beginResetModel();
    this->m_show_msg.clear();
    for (QList<LuaMessage>::iterator it = this->m_all_msg.begin(); it != this->m_all_msg.end(); ++it)
    {
        if (this->m_show_types.size() == 0 || this->m_show_types.contains((*it).Type()))
        {
            this->m_show_msg.append(*it);
            if (this->m_show_msg.count() >= this->m_max_row_count)
            {
                break;
            }
        }
    }
    this->endResetModel();
}

void LuaMessageListModel::clearDataAndTypes()
{
    QWriteLocker locker(&this->m_rw_lock);
    this->beginResetModel();
    this->m_show_msg.clear();
    this->m_all_msg.clear();
    this->m_show_types.clear();
    this->endResetModel();
}

Qt::ItemFlags LuaMessageListModel::flags(const QModelIndex &index) const
{
    return QAbstractListModel::flags(index) | Qt::ItemIsEditable;
}

void LuaMessageListModel::saveToFile()
{
    QString name = QFileDialog::getSaveFileName(0, QString::fromUtf8("保存"), "", "txt (*.txt);;all file (*.*)");
    if (name.isEmpty())
    {
        return;
    }
    QFile file(name);
    file.open(QFile::WriteOnly | QFile::Truncate | QFile::Text);
    QReadLocker locker(&this->m_rw_lock);
    for (int i = 0; i < this->m_show_msg.size(); ++i)
    {
        file.write((this->m_show_msg.at(i).Msg() + "\n").toUtf8());
    }
    file.close();
}
