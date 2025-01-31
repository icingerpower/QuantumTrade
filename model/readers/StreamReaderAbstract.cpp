#include "StreamReaderAbstract.h"

QList<const StreamReaderAbstract *> StreamReaderAbstract::ALL_STREAM_READERS;

StreamReaderAbstract::Recorder::Recorder(const StreamReaderAbstract *streamReader)
{
    StreamReaderAbstract::ALL_STREAM_READERS << streamReader;
}

StreamReaderAbstract::StreamReaderAbstract()
{

}

const QList<const StreamReaderAbstract *> &StreamReaderAbstract::allStreamReaders()
{
    return ALL_STREAM_READERS;
}

QMultiHash<QString, StreamReaderAbstract::VariableAvailability> StreamReaderAbstract::availableVariables(
    const Tick &tick) const
{
    QMultiHash<QString, StreamReaderAbstract::VariableAvailability> tickVariables;
    const auto &variables = availableVariables();
    for (auto it = variables.begin();
         it != variables.end(); ++it)
    {
        if (it.value().tickIds.contains(tick.id()))
        {
            tickVariables.insert(it.key(), it.value());
        }
    }
    return tickVariables;
}

/*
QMultiHash<QString, StreamReaderAbstract::VariableAvailability> StreamReaderAbstract::availableVariablesWithHistory() const
{
    QMultiHash<QString, StreamReaderAbstract::VariableAvailability> tickVariables;
    const auto &variables = availableVariables();
    for (auto it = variables.begin();
         it != variables.end(); ++it)
    {
        if (it.value().historicalData)
        {
            tickVariables.insert(it.key(), it.value());
        }
    }
    return tickVariables;
}

QMultiHash<QString, StreamReaderAbstract::VariableAvailability> StreamReaderAbstract::availableVariablesWithoutHistory() const
{
    QMultiHash<QString, StreamReaderAbstract::VariableAvailability> tickVariables;
    const auto &variables = availableVariables();
    for (auto it = variables.begin();
         it != variables.end(); ++it)
    {
        if (!it.value().historicalData)
        {
            tickVariables.insert(it.key(), it.value());
        }
    }
    return tickVariables;
}
//*/

/*
QSharedPointer<Job> StreamReaderAbstract::readLatestData(
    bool withoutHistoryOnly, QSharedPointer<Job> job)
{
    job->start();
    for (const auto &tick : Tick::TICKS)
    {
        auto availableVariables = StreamReaderAbstract::availableVariables(*tick);
        QList<VariableAbstract *> variables;
        for (auto availableVariable : std::as_const(availableVariables))
        {
            if (!withoutHistoryOnly ||  !availableVariable.historicalData)
            {
                variables << availableVariable.variable;
            }
        }
        readLatestData(*tick, variables, job);
    }
    return job;
}
//*/

