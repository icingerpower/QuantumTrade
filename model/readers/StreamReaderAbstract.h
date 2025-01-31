#ifndef STREAMREADERABSTRACT_H
#define STREAMREADERABSTRACT_H

#include <QSharedPointer>
#include <QString>
#include <QList>

#include "model/pairs/Tick.h"
#include "Job.h"

class VariableAbstract;

class StreamReaderAbstract
{
public:
    StreamReaderAbstract();
    static const QList<const StreamReaderAbstract *> & allStreamReaders();
    struct VariableAvailability{
        VariableAbstract *variable;
        QList<QString> tickIds;
        bool historicalData;
    };

    QMultiHash<QString, VariableAvailability> availableVariables(const Tick &tick) const;
    //*
    //QMultiHash<QString, VariableAvailability> availableVariablesWithHistory() const;
    //QMultiHash<QString, VariableAvailability> availableVariablesWithoutHistory() const;
    //virtual void readLatestDataWithoutHistory();

    QSharedPointer<Job> readLatestData(
        bool withoutHistoryOnly = false,
        QSharedPointer<Job> job = QSharedPointer<Job>{new Job});

    virtual QMultiHash<QString, VariableAvailability> availableVariables() const = 0;
    virtual QSharedPointer<Job> readLatestData(
        const Tick &tick,
        QList<VariableAbstract *> variables,
        QSharedPointer<Job> job = QSharedPointer<Job>{new Job}) = 0;
    virtual void readHistoricalData(
        const Tick &tick,
        VariableAbstract *variable,
        const QDate &dateFrom,
        const QDate &dateTo) = 0;
    virtual void readHistoricalData(
        const Tick &tick,
        QList<VariableAbstract *> variables,
        const QDate &dateFrom,
        const QDate &dateTo) = 0;


    class Recorder{
    public:
        Recorder(const StreamReaderAbstract *streamReader);
    };

private:
    static QList<const StreamReaderAbstract *> ALL_STREAM_READERS;
};

#endif // STREAMREADERABSTRACT_H
