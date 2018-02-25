#ifndef LUAVALUEEDIT_H
#define LUAVALUEEDIT_H

#include <QWidget>
#include <QTextEdit>
#include <QSettings>
class LuaValueEdit : public QWidget
{
    Q_OBJECT
public:
    explicit LuaValueEdit(QWidget *parent = 0);
    void saveConfig(QSettings &settings);
    void restoreConfig(QSettings &settings);
private:
    void setupUI();

    QTextEdit *m_editor;


signals:

public slots:
    void onTextChangedEvent();
    void onLuaStateChange(bool isRunning);
};

#endif // LUAVALUEEDIT_H
