#include "Tick.h"


Tick::Tick(const QString &id, const QString &name, const int &seconds)
{
    m_id = id;
    m_name = name;
    m_seconds = seconds;
}

const QString &Tick::id() const
{
    return m_id;
}

const QString &Tick::name() const
{
    return m_name;
}

int Tick::seconds() const
{
    return m_seconds;
}
