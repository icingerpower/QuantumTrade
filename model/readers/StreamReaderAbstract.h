#ifndef STREAMREADERABSTRACT_H
#define STREAMREADERABSTRACT_H

#include <QSharedPointer>
#include <QString>
#include <QList>
#include <QTimeZone>

#include "../common/utils/SortedMap.h"

#include "model/pairs/Tick.h"
#include "Job.h"
#include "VariableAvailablility.h"

class VariableAbstract;

class StreamReaderAbstract
{
public:
    struct Param{
        QString name;
        QVariant value;
    };
    static const QString PARAM_API_KEY;
    static const Param PARAM_API_KEY_NAME_VALUE;
    static const QString PARAM_ELASPED_MS;
    static const Param PARAM_ELASPED_MS_NAME_VALUE;
    static const QString PARAM_MAX_PER_DAY;
    static const Param PARAM_MAX_PER_DAY_NAME_VALUE;
    StreamReaderAbstract();
    static QStringList allSymbols();
    static const QMap<QString, VariableAvailability> &allAvailableVariables();
    static const QList<const StreamReaderAbstract *> & allStreamReaders();

    QMultiHash<QString, VariableAvailability> availableVariables(const Tick &tick) const;
    //*
    //QMultiHash<QString, VariableAvailability> availableVariablesWithHistory() const;
    //QMultiHash<QString, VariableAvailability> availableVariablesWithoutHistory() const;
    //virtual void readLatestDataWithoutHistory();

    //QSharedPointer<Job> readLatestData(
        //bool withoutHistoryOnly = false,
        //QSharedPointer<Job> job = QSharedPointer<Job>{new Job});

    QDateTime currentDateTime() const;
    virtual QString id() const = 0;
    virtual QString name() const = 0;
    virtual SortedMap<QString, Param> paramsDefault() const = 0;
    virtual QTimeZone timeZone() const = 0;

    virtual QMultiHash<QString, VariableAvailability> availableVariables() const = 0;
    virtual void readData(
            const SortedMap<QString, QVariant> &params,
            const Tick &tick,
            VariableAbstract *variable,
            const QDate &dateFrom,
            const QDate &dateTo,
            QSharedPointer<Job> job = QSharedPointer<Job>{new Job}) const = 0;
    virtual void readData(
            const SortedMap<QString, QVariant> &params,
            const Tick &tick,
            QList<VariableAbstract *> variables,
            const QDate &dateFrom,
            const QDate &dateTo,
            QSharedPointer<Job> job = QSharedPointer<Job>{new Job}) const;


    class Recorder{
    public:
        Recorder(const StreamReaderAbstract *streamReader);
    };

protected:
    static QVariant getSettingsValue(const QString &key,
                                     const QVariant &defaultValue = QVariant{});
    static void setSettingsValue(const QString &key, const QVariant &value);

private:
    static QList<const StreamReaderAbstract *> ALL_STREAM_READERS;
    static QMutex MUTEX;
};

#define RECORD_STREAMER(STREAMER_CLASS) \
STREAMER_CLASS instance##STREAMER_CLASS; \
    STREAMER_CLASS::Recorder recorder##STREAMER_CLASS{&instance##STREAMER_CLASS};


#endif // STREAMREADERABSTRACT_H
