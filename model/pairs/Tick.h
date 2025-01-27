#ifndef TICK_H
#define TICK_H

#include <QString>
#include <QTime>

class Tick
{
public:
    Tick(const QString &id, const QString &name, const int &seconds);

    const QString &id() const;

    const QString &name() const;

    int seconds() const;

private:
    QString m_id;
    QString m_name;
    int m_seconds;
};

#endif // TICK_H
