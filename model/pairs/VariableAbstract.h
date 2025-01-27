#ifndef VARIABLEABSTRACT_H
#define VARIABLEABSTRACT_H

#include <QTime>
#include <QSharedPointer>
#include <QSqlDatabase>

#include <thread>

#include "Tick.h"

class StreamReaderAbstract;

class VariableAbstract
{
public:
    static const Tick TICK_MIN_15;
    static const Tick TICK_HOUR_1;
    static const Tick TICK_HOUR_4;
    static const Tick TICK_DAY_1;
    struct TypeValue{
        QString id;
        QString name;
    };

    static const TypeValue TYPE_VALUE_OPEN;
    static const TypeValue TYPE_VALUE_CLOSE;
    static const TypeValue TYPE_VALUE_MIN;
    static const TypeValue TYPE_VALUE_MAX;
    static const TypeValue TYPE_VALUE_VOLUME;

    static const QList<TypeValue> TYPE_VALUES;
    static const QList<Tick> TICKS;

    static void setDatabaseFolder(const QString &dbFolderPath);
    static QSet<StreamReaderAbstract *> allStreamReaders();
    static void readAllStreams();
    static void stopReadingAllStreams();

    virtual QString name() const = 0;

    virtual StreamReaderAbstract *streamReader() const = 0; // Return null if no stream reader

    virtual bool canReadFromFile() const = 0;
    virtual QString fileExtentions() const = 0;
    virtual void importFromFile(const QString &filePath) = 0;

    class Recorder{
    public:
        Recorder(const VariableAbstract *variable);
    };
    QSharedPointer<std::vector<double>> readData(const Tick &tick,
            const QString &typeValueId,
            const QDateTime &dateFrom,
            const QDateTime &dateTo);
    QHash<QString, QSharedPointer<std::vector<double>>> readData(const Tick &tick,
            const QList<QString> &typeValueIds,
            const QDateTime &dateFrom,
            const QDateTime &dateTo);

protected:
    static QList<const VariableAbstract *> ALL_VARIABLES;
    void recordInDatabase(
            const QDateTime &dateTime,
            const Tick &tick,
            const QString &typeValueId,
            double value);
    void recordInDatabase(
            const QDateTime &dateTime,
            const Tick &tick,
            const QList<QString> &typeValueIds,
            const QList<double> values);

private:
    static std::vector<std::thread> THREADS;
    static QString DB_FOLDER_PATH;
    QSqlDatabase getDatabaseOpened(const QString &tickId) const;
};

#endif // VARIABLEABSTRACT_H
