#ifndef JOB_H
#define JOB_H

#include <QMutex>

class Job
{
public:
    Job();
    void start();
    bool isRunning();
    void stop();

private:
    bool m_running;
    QMutex m_mutex;
};

#endif // JOB_H
