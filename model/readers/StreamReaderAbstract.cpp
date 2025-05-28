#include "model/pairs/VariableAbstract.h"

#include "StreamReaderAbstract.h"

const StreamReaderAbstract::Param StreamReaderAbstract::PARAM_API_KEY_NAME_VALUE{
    QObject::tr("API Key"), QString{}};
const QString StreamReaderAbstract::PARAM_API_KEY{"apiKey"};

QList<const StreamReaderAbstract *> StreamReaderAbstract::ALL_STREAM_READERS;

StreamReaderAbstract::Recorder::Recorder(const StreamReaderAbstract *streamReader)
{
    StreamReaderAbstract::ALL_STREAM_READERS << streamReader;
}

StreamReaderAbstract::StreamReaderAbstract()
{

}

QStringList StreamReaderAbstract::allSymbols()
{
    QSet<QString> symbolsSet;
    for (const auto &streamReader : allStreamReaders())
    {
        auto streamSymbols = streamReader->availableVariables().uniqueKeys();
        for (const auto &symbol : streamSymbols)
        {
            symbolsSet.insert(symbol);
        }
    }
    QStringList symbols{symbolsSet.begin(), symbolsSet.end()};
    std::sort(symbols.begin(), symbols.end());
    return symbols;
}

QMap<QString, VariableAvailability> StreamReaderAbstract::allAvailableVariables()
{
    QMap<QString, VariableAvailability> variables;
    for (const auto &streamReader : allStreamReaders())
    {
        auto readerVariables = streamReader->availableVariables();
        for (const auto &readerVariable : readerVariables)
        {
            const QString &name = readerVariable.variable->name();
            if (!variables.contains(name))
            {
                variables[name] = readerVariable;
            }
            else
            {
                variables[name].tickIds.unite(readerVariable.tickIds);
            }
        }
    }
    return variables;
}

const QList<const StreamReaderAbstract *> &StreamReaderAbstract::allStreamReaders()
{
    return ALL_STREAM_READERS;
}

QMultiHash<QString, VariableAvailability> StreamReaderAbstract::availableVariables(
    const Tick &tick) const
{
    QMultiHash<QString, VariableAvailability> tickVariables;
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
QMultiHash<QString, VariableAvailability> StreamReaderAbstract::availableVariablesWithHistory() const
{
    QMultiHash<QString, VariableAvailability> tickVariables;
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

QMultiHash<QString, VariableAvailability> StreamReaderAbstract::availableVariablesWithoutHistory() const
{
    QMultiHash<QString, VariableAvailability> tickVariables;
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

