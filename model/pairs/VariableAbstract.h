#ifndef VARIABLEABSTRACT_H
#define VARIABLEABSTRACT_H

#include <QTime>
#include <QSharedPointer>
#include <QSqlDatabase>

#include <thread>

#include "Tick.h"

class VariableAbstract
{
public:

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

    static void setDatabaseFolder(const QString &dbFolderPath);
    //static std::vector<std::thread> THREADS;
    //static void readAllStreams();
    //static void stopReadingAllStreams();

    virtual QString name() const = 0;
    virtual QString nameDataBase() const = 0;

    QDateTime readDateTimeEnd(const Tick &tick) const;
    QDateTime readDateTimeStart(const Tick &tick) const;
    QSharedPointer<std::vector<double>> readData(const Tick &tick,
            const QString &typeValueId,
            const QDateTime &dateFrom,
            const QDateTime &dateTo);
    QHash<QString, QSharedPointer<std::vector<double>>> readData(const Tick &tick,
            const QList<QString> &typeValueIds,
            const QDateTime &dateFrom,
            const QDateTime &dateTo);

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
    static QString DB_FOLDER_PATH;
    QSqlDatabase getDatabaseOpened(const QString &tickId) const;
    QString colName(const QString &typeValueId) const;
};

#endif // VARIABLEABSTRACT_H
