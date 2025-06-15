#ifndef TICK_H
#define TICK_H

#include <QString>
#include <QTime>
#include <QComboBox>

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

    qint64 seconds() const;
    qint64 minutes() const;
    qint64 hours() const;

private:
    QString m_id;
    QString m_name;
    qint64 m_seconds;
};

class TickComboBox : public QComboBox
{
    Q_OBJECT
public:
    explicit TickComboBox(QWidget *parent = nullptr) : QComboBox(parent)
    {
        for (const Tick* tick : Tick::TICKS)
        {
            addItem(tick->name(), QVariant::fromValue(tick));
        }
    }
};


QDataStream & operator << (QDataStream &stream, const Tick *tick);
QDataStream & operator >> (QDataStream &stream, const Tick *&tick);

#endif // TICK_H
