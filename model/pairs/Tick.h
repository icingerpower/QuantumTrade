#ifndef TICK_H
#define TICK_H

#include <QString>
#include <QTime>

class Tick
{
public:
    Tick(const QString &id, const QString &name, const int &seconds);
    static const Tick TICK_MIN_1;
    static const Tick TICK_MIN_5;
    static const Tick TICK_MIN_15;
    static const Tick TICK_MIN_30;
    static const Tick TICK_HOUR_1;
    static const Tick TICK_DAY_1;
    static const QList<const Tick *> TICKS;
    static const QHash<QString, const Tick *> TICKS_BY_ID;

    const QString &id() const;

    const QString &name() const;

    int seconds() const;

private:
    QString m_id;
    QString m_name;
    int m_seconds;
};

#endif // TICK_H
