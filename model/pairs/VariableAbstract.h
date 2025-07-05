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
    static QDateTime dateTimeFromDb(const QVariant &variantDate);
    //static std::vector<std::thread> THREADS;
    //static void readAllStreams();
    //static void stopReadingAllStreams();

    virtual QString name() const = 0;
    virtual QString nameDataBase() const = 0;
    QString getTableName(const Tick &tick) const;

    QList<QDateTime> readDateTimeMissing(
        const QDate &start,
        const QDate &end,
        const Tick &tick) const;
    QList<QDate> readDaysMissing(
        const QList<QDateTime> &dateTimes) const;
    QList<QDate> readDaysMissing(
        const QDate &start,
        const QDate &end,
        const Tick &tick) const;
    QList<QDate> readMonthsMissing(
        const QList<QDateTime> &dateTimes) const;
    QList<QDate> readMonthsMissing(
        const QDate &start,
        const QDate &end,
        const Tick &tick) const;
    QDateTime readDateTimeEnd(const Tick &tick) const;
    QDateTime readDateTimeStart(const Tick &tick) const;
    QSharedPointer<QMap<QDateTime, double>> readData(
        const Tick &tick,
        const QString &typeValueId,
        const QDateTime &dateFrom,
        const QDateTime &dateTo);
    QSharedPointer<QHash<QString, QMap<QDateTime, double>>> readData(
        const Tick &tick,
        const QList<QString> &typeValueIds,
        const QDateTime &dateFrom,
        const QDateTime &dateTo) const;

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
    void closeDatabaseOpened(const QString &tickId) const;

private:
    static QString DB_FOLDER_PATH;
    QSqlDatabase getDatabaseOpened(const QString &tickId) const;
    QString colName(const QString &typeValueId) const;
    QString _getDatabaseConnection(const QString &tickId) const;
};

#endif // VARIABLEABSTRACT_H
