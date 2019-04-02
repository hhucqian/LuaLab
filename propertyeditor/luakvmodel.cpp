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
        this->beginResetModel();
        this->endResetModel();
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
    } else if (role == Qt::BackgroundRole) {
        if(this->m_colors.contains(this->m_keys.at(index.row()))) {
            return this->m_colors.value(this->m_keys.at(index.row()));
        }
    }
    return QVariant();
}

QVariant LuaKVModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        return this->m_headers.at(section);
    }
    return QVariant();
}

Qt::ItemFlags LuaKVModel::flags(const QModelIndex &/*index*/) const
{
    return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
}

void LuaKVModel::setKV(const QString &key, const QString &value, const QString &color)
{
    QWriteLocker(&this->m_lock);
    if(!this->m_keys.contains(key)) {
        this->m_keys.append(key);
    }
    this->m_values.insert(key, value);
    if(color.trimmed().length() == 0) {
        this->m_colors.remove(key);
    } else {
        QStringList rgb = color.split(',');
        while(rgb.length() < 3) {
            rgb.append("0");
        }
        if(rgb.length() == 3) {
            this->m_colors.insert(key, QBrush(QColor::fromRgb(rgb[0].toInt(), rgb[1].toInt(), rgb[2].toInt())));
        } else {
            this->m_colors.insert(key, QBrush(QColor::fromRgb(rgb[0].toInt(), rgb[1].toInt(), rgb[2].toInt(), rgb[3].toInt())));
        }
    }
    this->m_need_reset = true;
}

void LuaKVModel::clearKV()
{
    this->beginResetModel();
    this->m_keys.clear();
    this->m_values.clear();
    this->endResetModel();
}
