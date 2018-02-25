#ifndef LUAKVMODEL_H
#define LUAKVMODEL_H

#include <QAbstractTableModel>
#include <QList>
#include <QHash>
#include <QVariant>
#include <QReadWriteLock>
#include <QStringList>
#include <QTimerEvent>

class LuaKVModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit LuaKVModel(QObject *parent = 0);
    int columnCount(const QModelIndex &parent) const;
    int rowCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    void clearKV();
    void setKV(const QString &key, const QString &value);

signals:

protected:
    void timerEvent(QTimerEvent *event);

public slots:

private:
    QList<QString> m_keys;
    QHash<QString, QString> m_values;
    QStringList m_headers;

    QReadWriteLock m_lock;
    bool m_need_reset;

};

#endif // LUAKVMODEL_H
