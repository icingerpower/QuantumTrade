#ifndef StreamReaderAalphavantageStock_H
#define StreamReaderAalphavantageStock_H

#include <QList>
#include <future>
#include <QSharedPointer>

#include "StreamReaderAbstract.h"

class StreamReaderAalphavantageStock : public StreamReaderAbstract
{
public:
    StreamReaderAalphavantageStock();
    ~StreamReaderAalphavantageStock();
    QString id() const override;
    QString name() const override;
    SortedMap<QString, Param> paramsDefault() const override;
    QMultiHash<QString, VariableAvailability> availableVariables() const override;
    QTimeZone timeZone() const override;
    void readData(
        const SortedMap<QString, QVariant> &params,
        const Tick &tick,
        VariableAbstract * variable,
        const QDate &dateFrom,
        const QDate &dateTo,
        QSharedPointer<Job> job) const override;

private:
    static const QHash<QString, QString> HASH_TICK;
    static const QHash<QString, QString> HASH_VALUE_TYPES;
    mutable QList<QSharedPointer<std::future<void>>> m_futures;
    void _clearFinishedFutures() const;

};
//*/

#endif // StreamReaderAalphavantageStock_H
