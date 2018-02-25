#include "luakvmodel.h"

LuaKVModel::LuaKVModel(QObject *parent) : QAbstractTableModel(parent), m_need_reset(false)
{
    this->m_headers.append(QString::fromUtf8("键"));
    this->m_headers.append(QString::fromUtf8("值"));

    this->startTimer(50);
}

void LuaKVModel::timerEvent(QTimerEvent */*event*/)
{
    QWriteLocker(&this->m_lock);
    if(this->m_need_reset) {
        this->m_need_reset = false;
        this->reset();
    }
}

int LuaKVModel::rowCount(const QModelIndex &/*parent*/) const
{
    QReadLocker(const_cast<QReadWriteLock*>(&this->m_lock));
    return this->m_keys.size();
}

int LuaKVModel::columnCount(const QModelIndex &/*parent*/) const
{
    return this->m_headers.count();
}

QVariant LuaKVModel::data(const QModelIndex &index, int role) const
{
    QReadLocker(const_cast<QReadWriteLock*>(&this->m_lock));
    if(role == Qt::DisplayRole || role == Qt::EditRole) {
        if(index.column() == 0) {
            return this->m_keys.at(index.row());
        }
        if(index.column() == 1) {
            return this->m_values.value(this->m_keys.at(index.row()));
        }
    }
    return QVariant();
}

QVariant LuaKVModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        if(section == 0) {
            return this->m_headers.at(0);
        }
        if(section == 1) {
            return this->m_headers.at(1);
        }
    }
    return QVariant();
}

bool LuaKVModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    QWriteLocker(&this->m_lock);
    if(role == Qt::EditRole) {
        QWriteLocker(&this->m_lock);
        this->m_values.insert(this->m_keys.at(index.row()), value.toString());
        emit dataChanged(index, index);
        return true;
    }
    return false;
}

Qt::ItemFlags LuaKVModel::flags(const QModelIndex &/*index*/) const
{
    return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
}

void LuaKVModel::setKV(const QString &key, const QString &value)
{
    QWriteLocker(&this->m_lock);
    if(!this->m_keys.contains(key)) {
        this->m_keys.append(key);
    }
    this->m_values.insert(key, value);
    this->m_need_reset = true;
}

void LuaKVModel::clearKV()
{
    this->m_keys.clear();
    this->m_values.clear();
    this->reset();
}
