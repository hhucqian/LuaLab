#ifndef LLPROPERTYEDITOR_H
#define LLPROPERTYEDITOR_H

#include <QWidget>
#include <QTableView>

class LuaKVModel;

class PropertyEditor : public QWidget
{
    Q_OBJECT
public:
    explicit PropertyEditor(QWidget *parent = 0);

public slots:
    void onSetKV(QString key, QString value);
    void onLuaStateChange(QString, bool isRunning);

protected:

private:
    void setupUI();

    QTableView *m_table_view;
    LuaKVModel *m_model;

signals:

};

#endif // LLPROPERTYEDITOR_H
