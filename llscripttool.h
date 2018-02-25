#ifndef LLSCRIPTTOOL_H
#define LLSCRIPTTOOL_H

#include <QWidget>
#include <QPushButton>
#include <QComboBox>
#include <QVector>
#include <QSettings>

class LLScriptTool : public QWidget
{
    Q_OBJECT
public:
    explicit LLScriptTool(QWidget *parent = 0);
    void saveConfig(QSettings &settings);
    void restoreConfig(QSettings &settings);
private:
    void setupUI();
    void updateUI();
    QVector<QString> getScriptFileNames();
    bool m_is_pause;

    QPushButton *m_btn_run, *m_btn_pause, *m_btn_edit;
    QComboBox *m_cbx_scripts;

signals:

public slots:
    void onRunClick();
    void onPauseClick();
    void onEditClick();
    void onLuaStateChange(bool isRunning);
};

#endif // LLSCRIPTTOOL_H
