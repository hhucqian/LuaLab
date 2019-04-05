#include "sleeperthread.h"

SleeperThread::SleeperThread(QObject *parent) : QThread(parent)
{
}

void SleeperThread::msleep(unsigned long msecs)
{
    QThread::msleep(msecs);
}

void SleeperThread::usleep(unsigned long usecs)
{
    QThread::usleep(usecs);
}

void SleeperThread::sleep(unsigned long secs)
{
    QThread::sleep(secs);
}
