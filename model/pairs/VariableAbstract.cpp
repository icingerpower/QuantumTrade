#include <QSqlQuery>
#include <QThread>
#include <QSqlError>
#include <QSqlRecord>
#include <QObject>
#include <QDir>
#include <QDebug>

#include "ExceptionVariableDb.h"

#include "VariableAbstract.h"

const VariableAbstract::TypeValue VariableAbstract::TYPE_VALUE_OPEN{"Open", QObject::tr("Open")};
const VariableAbstract::TypeValue VariableAbstract::TYPE_VALUE_CLOSE{"Close", QObject::tr("Close")};
const VariableAbstract::TypeValue VariableAbstract::TYPE_VALUE_MIN{"Min", QObject::tr("Min")};
const VariableAbstract::TypeValue VariableAbstract::TYPE_VALUE_MAX{"Max", QObject::tr("Max")};
const VariableAbstract::TypeValue VariableAbstract::TYPE_VALUE_VOLUME{"Volume", QObject::tr("Volume")};
const QList<VariableAbstract::TypeValue> VariableAbstract::TYPE_VALUES{
    TYPE_VALUE_OPEN
    , TYPE_VALUE_CLOSE
    , TYPE_VALUE_MIN
    , TYPE_VALUE_MAX
    , TYPE_VALUE_VOLUME
};

QString VariableAbstract::DB_FOLDER_PATH;

void VariableAbstract::setDatabaseFolder(const QString &dbFolderPath)
{
    DB_FOLDER_PATH = dbFolderPath;
}

QDateTime VariableAbstract::dateTimeFromDb(const QVariant &variantDate)
{
    const auto &maxDateStringFull = variantDate.toString();
    const auto &maxDateStringWithoutTimespec = maxDateStringFull.left(
        maxDateStringFull.size()-6);
    const auto &timeZone = maxDateStringFull.right(6);
    auto dataTime = QDateTime::fromString(
        maxDateStringWithoutTimespec, "yyyy-MM-ddThh:mm:ss");
    int sign      = (timeZone[0] == QLatin1Char('-') ? -1 : 1);
    int hours     = timeZone.mid(1,2).toInt();
    int minutes   = timeZone.mid(4,2).toInt();
    int offsetSec = sign * (hours*3600 + minutes*60);

    // 3) tell Qt “this QDateTime is offsetFromUTC by offsetSec”
    QTimeZone zone = QTimeZone::fromSecondsAheadOfUtc(offsetSec);
    dataTime.setTimeZone(zone);
    return dataTime;
}

QList<QDateTime> VariableAbstract::readDateTimeMissing(
    const QDate &start, const QDate &end, const Tick &tick) const
{
    QList<QDateTime> missingList;

    // 1. Open the database connection
    QSqlDatabase db = getDatabaseOpened(tick.id());
    if (!db.isOpen())
    {
        qWarning() << "Database is not open for Tick ID:" << tick.id();
        return missingList;
    }

    // 2. Define the table name
    const QString tableName = QStringLiteral("measurements");
    QSet<QDateTime> recorded;

    QDateTime dtStart(start, QTime(0, 0, 0));
    QDateTime dtEnd(end,   QTime(23, 59, 59));

    if (db.tables().contains(tableName))
    {
        // 3. Query for all recorded dateTime values in that range
        QString selectQueryStr =
            QString("SELECT dateTime FROM %1 "
                    "WHERE dateTime BETWEEN :start AND :end "
                    "ORDER BY dateTime ASC")
                .arg(tableName);

        QSqlQuery query(db);
        if (!query.prepare(selectQueryStr))
        {
            qWarning() << "Error preparing readDateTimeMissing query:"
                       << query.lastError().text();
            return missingList;
        }
        query.bindValue(":start", dtStart.toString(Qt::ISODate));
        query.bindValue(":end",   dtEnd.toString(Qt::ISODate));

        if (!query.exec())
        {
            qWarning() << "Error executing readDateTimeMissing query:"
                       << query.lastError().text();
            return missingList;
        }

        // 4. Collect all recorded timestamps into a set
        while (query.next())
        {
            const auto &date = query.value(0);
            const QDateTime &dt = dateTimeFromDb(date);
            if (dt.isValid())
            {
                recorded.insert(dt);
            }
        }
    }

    // 5. Step through the full expected sequence and find gaps
    const qint64 step = tick.seconds();
    for (QDateTime cur = dtStart; cur <= dtEnd; cur = cur.addSecs(step))
    {
        if (!recorded.contains(cur))
        {
            missingList.append(QDateTime(cur.date(), cur.time()));
        }
        else
        {
            int TEMP=0;++TEMP;
        }
    }
    /*
    for (const auto &missing : missingList)
    {
        auto TEMP=missing;
        qDebug() << missing;
    }
//*/

    return missingList;
}

QList<QDate> VariableAbstract::readDaysMissing(const QList<QDateTime> &dateTimes) const
{
    QHash<int, QHash<int, int>> year_month_day;
    for (const auto &dateTime : dateTimes)
    {
        const auto &date = dateTime.date();
        year_month_day[date.year()][date.month()] = date.day();
    }
    QList<QDate> dates;
    for (auto itYear = year_month_day.begin();
         itYear != year_month_day.end(); ++itYear)
    {
        for (auto itMonthDay = itYear.value().begin();
             itMonthDay != itYear.value().end(); ++itMonthDay)
        {
            dates << QDate{itYear.key(), itMonthDay.key(), itMonthDay.value()};
        }
    }
    return dates;
}

QList<QDate> VariableAbstract::readDaysMissing(
    const QDate &start, const QDate &end, const Tick &tick) const
{
    return readDaysMissing(readDateTimeMissing(start, end, tick));
}

QList<QDate> VariableAbstract::readMonthsMissing(const QList<QDateTime> &dateTimes) const
{
    QMap<int, QMap<int, int>> year_month;
    for (const auto &dateTime : dateTimes)
    {
        const auto &date = dateTime.date();
        year_month[date.year()][date.month()] = 1;
    }
    QList<QDate> dates;
    for (auto itYear = year_month.begin();
         itYear != year_month.end(); ++itYear)
    {
        for (auto itMonth = itYear.value().begin();
             itMonth != itYear.value().end(); ++itMonth)
        {
            dates << QDate{itYear.key(), itMonth.key(), 1};
        }
    }
    return dates;
}

QList<QDate> VariableAbstract::readMonthsMissing(
    const QDate &start, const QDate &end, const Tick &tick) const
{
    return readMonthsMissing(readDateTimeMissing(start, end, tick));
}

QDateTime VariableAbstract::readDateTimeEnd(const Tick &tick) const
{
    QDateTime lastDateTime;
    QSqlDatabase db = getDatabaseOpened(tick.id());
    if (!db.isOpen())
    {
        qWarning() << "Database is not open for Tick ID:" << tick.id();
        return lastDateTime; // Returns an invalid QDateTime
    }

    // 2. Define the table name (ensure it matches the one used in recordInDatabase)
    const QString tableName = QStringLiteral("measurements");

    // 3. Check if the table exists
    if (!db.tables().contains(tableName))
    {
        qWarning() << "Table" << tableName << "does not exist in the database.";
        return lastDateTime; // Returns an invalid QDateTime
    }

    // 4. Prepare the SELECT MAX(dateTime) query
    QString selectMaxQueryStr = QString("SELECT MAX(dateTime) FROM %1").arg(tableName);

    QSqlQuery query(db);
    if (!query.exec(selectMaxQueryStr))
    {
        qWarning() << "Error executing SELECT MAX query:" << query.lastError().text();
        return lastDateTime; // Returns an invalid QDateTime
    }

    // 5. Process the query result
    if (query.next())
    {
        QVariant maxDateVariant = query.value(0);
        if (!maxDateVariant.isNull())
        {
            // Convert the retrieved string to QDateTime using ISO8601 format
            lastDateTime = QDateTime::fromString(maxDateVariant.toString(), Qt::ISODate);
            if (!lastDateTime.isValid())
            {
                qWarning() << "Invalid dateTime format retrieved:" << maxDateVariant.toString();
                // Optionally, you can handle this case differently, e.g., throw an exception
            }
        }
        else
        {
            qWarning() << "No records found in table" << tableName << "for Tick ID:" << tick.id();
            // lastDateTime remains invalid
        }
    }
    else
    {
        qWarning() << "Failed to retrieve MAX(dateTime) from table" << tableName;
        // lastDateTime remains invalid
    }
    return lastDateTime;
}

QDateTime VariableAbstract::readDateTimeStart(const Tick &tick) const
{
    QDateTime lastDateTime;
    QSqlDatabase db = getDatabaseOpened(tick.id());
    if (!db.isOpen())
    {
        qWarning() << "Database is not open for Tick ID:" << tick.id();
        return lastDateTime; // Returns an invalid QDateTime
    }

    // 2. Define the table name (ensure it matches the one used in recordInDatabase)
    const QString tableName = QStringLiteral("measurements");

    // 3. Check if the table exists
    if (!db.tables().contains(tableName))
    {
        qWarning() << "Table" << tableName << "does not exist in the database.";
        return lastDateTime; // Returns an invalid QDateTime
    }

    // 4. Prepare the SELECT MAX(dateTime) query
    QString selectMaxQueryStr = QString("SELECT MIN(dateTime) FROM %1").arg(tableName);

    QSqlQuery query(db);
    if (!query.exec(selectMaxQueryStr))
    {
        qWarning() << "Error executing SELECT MIN query:" << query.lastError().text();
        return lastDateTime; // Returns an invalid QDateTime
    }

    // 5. Process the query result
    if (query.next())
    {
        QVariant maxDateVariant = query.value(0);
        if (!maxDateVariant.isNull())
        {
            lastDateTime = dateTimeFromDb(maxDateVariant);
            // Convert the retrieved string to QDateTime using ISO8601 format
            if (!lastDateTime.isValid())
            {
                qWarning() << "Invalid dateTime format retrieved:" << maxDateVariant.toString();
                // Optionally, you can handle this case differently, e.g., throw an exception
            }
        }
        else
        {
            qWarning() << "No records found in table" << tableName << "for Tick ID:" << tick.id();
            // lastDateTime remains invalid
        }
    }
    else
    {
        qWarning() << "Failed to retrieve MAX(dateTime) from table" << tableName;
        // lastDateTime remains invalid
    }
    return lastDateTime;
}

QSharedPointer<QMap<QDateTime, double>> VariableAbstract::readData(
    const Tick &tick,
    const QString &typeValueId,
    const QDateTime &dateFrom,
    const QDateTime &dateTo)
{
    QSharedPointer<QMap<QDateTime, double>> data{new QMap<QDateTime, double>};

    QSqlDatabase db = getDatabaseOpened(tick.id());
    if (!db.isOpen())
    {
        qWarning() << "Database is not open!";
        return data;
    }

    // 2. Define the table name (ensure it matches the one used in recordInDatabase)
    const QString tableName = QStringLiteral("measurements");

    // 3. Check if the column exists
    const QString &colName = this->colName(typeValueId);
    QSqlRecord tableRecord = db.record(tableName);
    if (tableRecord.indexOf(colName) == -1)
    {
        qWarning() << "Column" << typeValueId << "does not exist in table" << tableName;
        return data;
    }

    // 4. Prepare the SELECT query
    //    Select the value from the specified column within the date range
    QString selectQueryStr = QString(
        "SELECT dateTime, \"%1\" FROM %2 "
        "WHERE dateTime BETWEEN :dateFrom AND :dateTo "
        "ORDER BY dateTime ASC"
    ).arg(typeValueId, tableName);

    QSqlQuery selectQuery(db);
    if (!selectQuery.prepare(selectQueryStr))
    {
        qWarning() << "Error preparing SELECT query:" << selectQuery.lastError().text();
        return data;
    }

    // Bind the date parameters using ISO8601 format
    selectQuery.bindValue(":dateFrom", dateFrom.toString(Qt::ISODate));
    selectQuery.bindValue(":dateTo", dateTo.toString(Qt::ISODate));

    // 5. Execute the query
    if (!selectQuery.exec())
    {
        qWarning() << "Error executing SELECT query:" << selectQuery.lastError().text();
        return data;
    }

    // 6. Process the results
    while (selectQuery.next())
    {
        const auto &dateTimeVariant = selectQuery.value(0);
        const auto &dateTime = dateTimeFromDb(dateTimeVariant);
        QVariant valueVariant = selectQuery.value(1);
        if (valueVariant.isNull())
        {
            (*data)[dateTime] = 0.;
            continue;
        }

        bool ok = false;
        double value = valueVariant.toDouble(&ok);
        if (ok)
        {
            (*data)[dateTime] = value;
        }
        else
        {
            qWarning() << "Invalid double value encountered:" << valueVariant;
            // Optionally handle the error, e.g., skip the value or insert a default
        }
    }
    return data;
}

QSharedPointer<QHash<QString, QMap<QDateTime, double> > > VariableAbstract::readData(
        const Tick &tick,
        const QList<QString> &typeValueIds,
        const QDateTime &dateFrom,
    const QDateTime &dateTo) const
{
    QSqlDatabase db = getDatabaseOpened(tick.id());
    if (!db.isOpen())
    {
        qWarning() << "Database is not open!";
        // Handle error or throw an exception
        return {};
    }

    // 2. Ensure the table (e.g. "measurements") exists.
    //    Adjust the schema to your needs if necessary.
    const QString tableName = QStringLiteral("measurements");
    {
        QString createTableQueryStr = QString(
            "CREATE TABLE IF NOT EXISTS %1 ("
            "  dateTime TEXT PRIMARY KEY"
            ")"
        ).arg(tableName);

        QSqlQuery createTableQuery(db);
        if (!createTableQuery.exec(createTableQueryStr))
        {
            qWarning() << "Error creating table:" << createTableQuery.lastError().text();
            // Handle error or throw an exception
            return {};
        }
    }

    // 4. Build a single SELECT query that retrieves all requested columns
    //    plus dateTime, filtered by [dateFrom, dateTo].
    //    We'll store results in a hash of vectors, keyed by column name.
    QSharedPointer<QHash<QString, QMap<QDateTime, double>>> typeValue_date_value
        {new QHash<QString, QMap<QDateTime, double>>{}};
    QStringList colNames;
    for (const QString &typeValueId : typeValueIds)
    {
        colNames << colName(typeValueId);
    }

    // Construct the column list for the SELECT statement
    // We also select "dateTime" in case you need it for debugging or further usage.
    QStringList columnsToSelect;
    columnsToSelect << "dateTime"; // Not strictly necessary if you only need numeric data
    for (const QString &colName : colNames)
    {
        // Quote the column name in case it has special characters or spaces
        columnsToSelect << QString("\"%1\"").arg(colName);
    }

    // The BETWEEN operator in SQLite is inclusive: dateTime >= :dateFrom AND dateTime <= :dateTo
    // If your dateTime strings are stored in ISO8601 format, lexicographical comparisons will work properly.
    QString selectQueryStr = QString(
        "SELECT %1 FROM %2 "
        "WHERE dateTime BETWEEN :dateFrom AND :dateTo "
        "ORDER BY dateTime"
    ).arg(columnsToSelect.join(", "), tableName);

    QSqlQuery selectQuery(db);
    if (!selectQuery.prepare(selectQueryStr))
    {
        qWarning() << "Error preparing select query:" << selectQuery.lastError().text();
        // Handle error or throw an exception
        return {};
    }

    // Bind date range. Store your dateTimes in ISO8601 format for safe lexicographical comparison.
    selectQuery.bindValue(":dateFrom", dateFrom.toString(Qt::ISODate));
    selectQuery.bindValue(":dateTo", dateTo.toString(Qt::ISODate));

    if (!selectQuery.exec())
    {
        qWarning() << "Error executing select query:" << selectQuery.lastError().text();
        // Handle error or throw an exception
        return {};
    }

    while (selectQuery.next())
    {
        // If you need `dateTime` from the DB, you can parse it here:
        // QString dbDateTime = selectQuery.value(0).toString();

        const auto &dateTimeVariant = selectQuery.value(0);
        const auto &dateTime = dateTimeFromDb(dateTimeVariant);
        // For each column in typeValueIds, read the corresponding field.
        for (int i = 0; i < typeValueIds.size(); ++i)
        {
            const QString &colName = colNames.at(i);
            double val = selectQuery.value(i + 1).toDouble(); // +1 because index 0 is dateTime
            (*typeValue_date_value)[colName][dateTime] = val;
        }
    }

    // 6. Return the hash of results
    return typeValue_date_value;
}

void VariableAbstract::recordInDatabase(
        const QDateTime &dateTime,
        const Tick &tick,
        const QString &typeValueId,
        double value)
{
    QSqlDatabase db = getDatabaseOpened(tick.id());
    if (!db.isOpen())
    {
        // Handle error if needed, e.g. throw an exception
        qWarning() << "Database is not open!";
        return;
    }

    // 2. Ensure the table (e.g. "measurements") exists.
    //    Adjust the table name and schema to your needs.
    const QString tableName = QStringLiteral("measurements");
    QString createTableQueryStr = QString(
        "CREATE TABLE IF NOT EXISTS %1 ("
        "  dateTime TEXT PRIMARY KEY" // dateTime stored as ISO8601 string
        ")"
    ).arg(tableName);

    QSqlQuery createTableQuery(db);
    if (!createTableQuery.exec(createTableQueryStr))
    {
        qWarning() << "Error creating table:" << createTableQuery.lastError().text();
        // Handle error if needed, e.g. throw an exception
        return;
    }

    const QString &colName = name() + typeValueId;
    QSqlRecord tableRecord = db.record(tableName);
    if (tableRecord.indexOf(typeValueId) == -1)
    {
        QString alterTableQueryStr = QString(
            "ALTER TABLE %1 "
            "ADD COLUMN \"%2\" REAL"
        ).arg(tableName, typeValueId);

        QSqlQuery alterTableQuery(db);
        if (!alterTableQuery.exec(alterTableQueryStr))
        {
            qWarning() << "Error adding column:" << alterTableQuery.lastError().text();
            // Handle error if needed
            return;
        }
    }

    // 4. Perform an upsert to set the column value for the given dateTime.
    //    We use 'ON CONFLICT(dateTime) DO UPDATE ...' to update if the row already exists.
    QString insertQueryStr = QString(
        "INSERT INTO %1 (dateTime, \"%2\") "
        "VALUES (:dateTime, :value) "
        "ON CONFLICT(dateTime) DO UPDATE SET \"%2\" = excluded.\"%2\""
    ).arg(tableName, typeValueId);

    QSqlQuery insertQuery(db);
    if (!insertQuery.prepare(insertQueryStr))
    {
        qWarning() << "Error preparing insert query:" << insertQuery.lastError().text();
        // Handle error if needed
        return;
    }

    // Use ISO8601 string for dateTime or any format you prefer
    insertQuery.bindValue(":dateTime", dateTime.toString(Qt::ISODate));
    insertQuery.bindValue(":value", value);

    if (!insertQuery.exec())
    {
        qWarning() << "Error executing insert query:" << insertQuery.lastError().text();
        // Handle error if needed
        return;
    }
}

void VariableAbstract::recordInDatabase(
        const QDateTime &dateTime,
        const Tick &tick,
        const QList<QString> &typeValueIds,
        const QList<double> values)
{
    QSqlDatabase db = getDatabaseOpened(tick.id());
    if (!db.isOpen())
    {
        qWarning() << "Database is not open!";
        return;
    }

    // 2. Ensure the table (e.g. "measurements") exists
    const QString tableName = QStringLiteral("measurements");
    QString createTableQueryStr = QString(
        "CREATE TABLE IF NOT EXISTS %1 ("
        "  dateTime TEXT PRIMARY KEY"  // Primary key to prevent duplicates
        ")"
    ).arg(tableName);

    QSqlQuery createTableQuery(db);
    if (!createTableQuery.exec(createTableQueryStr))
    {
        ExceptionVariableDb exception;
        exception.setTitle(QObject::tr("Can’t write database"));
        exception.setError(QObject::tr("Can’t create table in the database due to the following error")
                           + ": " + createTableQuery.lastError().text());
        exception.raise();
        qWarning() << "Error creating table:" << createTableQuery.lastError().text();
        return;
    }

    // 3. Ensure columns exist for each typeValueId
    QStringList colNames;
    QSqlRecord tableRecord = db.record(tableName);
    for (const QString &typeValueId : typeValueIds)
    {
        const auto &colName = this->colName(typeValueId);
        colNames << colName;
        if (tableRecord.indexOf(typeValueId) == -1)
        {
            QString alterTableQueryStr = QString(
                "ALTER TABLE %1 "
                "ADD COLUMN \"%2\" REAL"
            ).arg(tableName, colName);

            QSqlQuery alterTableQuery(db);
            if (!alterTableQuery.exec(alterTableQueryStr))
            {
                ExceptionVariableDb exception;
                exception.setTitle(QObject::tr("Can’t write database"));
                exception.setError(QObject::tr("Can’t add a column in the database due to the following error")
                                   + ": " + alterTableQuery.lastError().text());
                exception.raise();
                qWarning() << "Error adding column:" << alterTableQuery.lastError().text();
                return;
            }
        }
    }

    // 4. Perform an upsert to set multiple column values for the given dateTime
    // Construct query dynamically based on typeValueIds
    QStringList columnNames;
    QStringList valuePlaceholders;
    for (const QString &colName : colNames)
    {
        columnNames << QString("\"%1\"").arg(colName);  // Protect against SQL injection
        valuePlaceholders << QString(":%1").arg(colName);
    }

    QString insertQueryStr = QString(
        "INSERT INTO %1 (dateTime, %2) "
        "VALUES (:dateTime, %3) "
        "ON CONFLICT(dateTime) DO UPDATE SET %4"
    ).arg(tableName,
          columnNames.join(", "),         // Columns for insertion
          valuePlaceholders.join(", "),    // Value placeholders
          columnNames.join(" = excluded.") + " = excluded." + columnNames.join(", excluded."));

    QSqlQuery insertQuery(db);
    if (!insertQuery.prepare(insertQueryStr))
    {
        qWarning() << "Error preparing insert query:" << insertQuery.lastError().text();
        return;
    }

    // Bind values to the query
    insertQuery.bindValue(":dateTime", dateTime.toString(Qt::ISODate));
    for (int i = 0; i < colNames.size(); ++i)
    {
        insertQuery.bindValue(":" + colNames[i], values[i]);
    }

    // Execute the query
    if (!insertQuery.exec())
    {
        qWarning() << "Error executing insert query:" << insertQuery.lastError().text();
        return;
    }
    qDebug() << "Data successfully recorded for dateTime:" << dateTime.toString(Qt::ISODate);
}

QSqlDatabase VariableAbstract::getDatabaseOpened(const QString &tickId) const
{
    const QString &connectionName = _getDatabaseConnection(tickId);
    if (!QSqlDatabase::contains(connectionName))
    {
        if (DB_FOLDER_PATH.isEmpty())
        {
            ExceptionVariableDb exception;
            exception.setTitle(QObject::tr("No database folder"));
            exception.setError(QObject::tr("The database folder was not initialized."));
            exception.raise();
        }
        Q_ASSERT(!DB_FOLDER_PATH.isEmpty()); // TODO Exception if empty
        QString dbFilePath = QDir{DB_FOLDER_PATH}.absoluteFilePath(name() + ".db");;
        qDebug() << "Opening sqlite database:" << dbFilePath;

        // Add a database connection with a unique connection name
        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", connectionName);
        db.setDatabaseName(dbFilePath); // Set the database file path

        if (!db.open())
        {
            ExceptionVariableDb exception;
            exception.setTitle(QObject::tr("Can’t open database"));
            exception.setError(QObject::tr("The database can’t be opened."));
            exception.raise();
        }
        return db;
    }
    return QSqlDatabase::database(connectionName);
}

void VariableAbstract::closeDatabaseOpened(const QString &tickId) const
{
    const QString &connectionName = _getDatabaseConnection(tickId);
    auto db = getDatabaseOpened(tickId);
    db.close();
    QSqlDatabase::removeDatabase(connectionName);
}

QString VariableAbstract::colName(const QString &typeValueId) const
{
    return typeValueId;
}

QString VariableAbstract::_getDatabaseConnection(const QString &tickId) const
{
    return nameDataBase() + "_" + tickId + QString::number((quintptr)QThread::currentThread());
}

