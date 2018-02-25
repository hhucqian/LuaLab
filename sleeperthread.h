#ifndef SLEEPERTHREAD_H
#define SLEEPERTHREAD_H

#include <QThread>

class SleeperThread : public QThread
{
    Q_OBJECT
public:
    explicit SleeperThread(QObject *parent = 0);

    static void msleep(unsigned long msecs);
    static void usleep(unsigned long usecs);
    static void sleep(unsigned long secs);

signals:

public slots:
};

#endif // SLEEPERTHREAD_H
