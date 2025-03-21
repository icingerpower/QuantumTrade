#include <QItemEditorFactory>
#include <QStandardItemEditorCreator>

#include "Tick.h"

const Tick Tick::TICK_MIN_1{"1min", QObject::tr("1 minute"), 15*60};
const Tick Tick::TICK_MIN_5{"5min", QObject::tr("5 minutes"), 15*60};
const Tick Tick::TICK_MIN_15{"15min", QObject::tr("15 minutes"), 15*60};
const Tick Tick::TICK_MIN_30{"30min", QObject::tr("30 minutes"), 30*60};
const Tick Tick::TICK_HOUR_1{"1hour", QObject::tr("1 hour"), 60*60};
const Tick Tick::TICK_DAY_1{"1day", QObject::tr("1 day"), 60*60*24};
const QList<const Tick *> Tick::TICKS{
    &TICK_MIN_1
    , &TICK_MIN_5
    , &TICK_MIN_15
    , &TICK_MIN_30
    , &TICK_HOUR_1
    , &TICK_DAY_1
};

const QHash<QString, const Tick *> Tick::TICKS_BY_ID = []() -> QHash<QString, const Tick *>{
    QHash<QString, const Tick *> _TICKS_BY_ID;
    for (const auto &tick : TICKS)
    {
        _TICKS_BY_ID[tick->id()] = tick;
    }
    return _TICKS_BY_ID;
}();

QDataStream & operator << (QDataStream &stream, const Tick *tick)
{
    stream << tick->id();
    return stream;
}

QDataStream & operator >> (QDataStream &stream, const Tick *&tick)
{
    QString id;
    stream >> id;
    tick = Tick::TICKS_BY_ID[id];
    return stream;
}

Tick::Tick(const QString &id, const QString &name, const int &seconds)
{
    static bool registred = []() -> bool {
        qRegisterMetaType<const Tick *>();
        return true;
    }();
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

