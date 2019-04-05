#ifndef LLPROPERTYEDITOR_H
#define LLPROPERTYEDITOR_H

#include <QTableView>
#include <QWidget>

class LuaKVModel;

class PropertyEditor : public QWidget
{
    Q_OBJECT
  public:
    explicit PropertyEditor(QWidget *parent = 0);

  public slots:
    void onSetKV(QString key, QString value, QString color);
    void onLuaStateChange(QString, bool isRunning);

  protected:
  private:
    void setupUI();

    QTableView *m_table_view;
    LuaKVModel *m_model;

  signals:
};

#endif // LLPROPERTYEDITOR_H
