#include <QMutexLocker>

#include "Job.h"

Job::Job() {}

void Job::start()
{
    m_mutex.lock();
    m_running = true;
    m_mutex.unlock();
}

bool Job::isRunning()
{
    QMutexLocker mutexLocked(&m_mutex);
    return m_running;
}

void Job::stop()
{
    m_mutex.lock();
    m_running = false;
    m_mutex.unlock();
}
