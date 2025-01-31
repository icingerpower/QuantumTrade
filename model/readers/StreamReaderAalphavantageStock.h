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
    QMultiHash<QString, VariableAvailability> availableVariables() const override;
    QSharedPointer<Job> readLatestData(
        const Tick &tick,
        QList<VariableAbstract *> variables,
        QSharedPointer<Job> job = QSharedPointer<Job>{new Job}) override;
    void readHistoricalData(
        const Tick &tick,
        VariableAbstract *variable,
        const QDate &dateFrom,
        const QDate &dateTo) override;
    void readHistoricalData(
        const Tick &tick,
        QList<VariableAbstract *> variables,
        const QDate &dateFrom,
        const QDate &dateTo) override;

private:
    static const QHash<QString, QString> HASH_TICK;
    static const QHash<QString, QString> HASH_VALUE_TYPES;
    QList<QSharedPointer<std::future<void>>> m_futures;
    QList<QDate> generateMonths(const QDate &startDate, const QDate &endDate) const;
    void _clearFinishedFutures();

};
//*/

#endif // StreamReaderAalphavantageStock_H
