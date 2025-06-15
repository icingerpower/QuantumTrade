#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QEventLoop>
#include <QThread>
#include <QTimeZone>
#include <QSettings>
#include <QDateTime>
#include <QUrlQuery>

#include <future>

#include "model/pairs/Stock.h"
#include "model/pairs/Crypto.h"
#include "model/pairs/Currency.h"

#include "StreamReaderAalphavantageStock.h"

RECORD_STREAMER(StreamReaderAalphavantageStock)

StreamReaderAalphavantageStock::StreamReaderAalphavantageStock()
    : StreamReaderAbstract()
{
}

QString StreamReaderAalphavantageStock::id() const
{
    return "StreamReaderAlphavantageStock";
}

QString StreamReaderAalphavantageStock::name() const
{
    return "Alphavantage";
}

SortedMap<QString, StreamReaderAbstract::Param> StreamReaderAalphavantageStock::paramsDefault() const
{
    SortedMap<QString, Param> params;
    params[PARAM_API_KEY] = PARAM_API_KEY_NAME_VALUE;
    params[PARAM_ELASPED_MS] = PARAM_ELASPED_MS_NAME_VALUE;
    params[PARAM_MAX_PER_DAY] = PARAM_MAX_PER_DAY_NAME_VALUE;
    return params;
}

const QHash<QString, QString> StreamReaderAalphavantageStock::HASH_TICK{
      {Tick::TICK_MIN_1.id(), "1min"}
    , {Tick::TICK_MIN_5.id(), "5min"}
    , {Tick::TICK_MIN_15.id(), "15min"}
    , {Tick::TICK_MIN_30.id(), "30min"}
    , {Tick::TICK_HOUR_1.id(), "60min"}
};

const QHash<QString, QString> StreamReaderAalphavantageStock::HASH_VALUE_TYPES{
      {VariableAbstract::TYPE_VALUE_OPEN.id, "1. open"}
    , {VariableAbstract::TYPE_VALUE_MAX.id, "2. high"}
    , {VariableAbstract::TYPE_VALUE_MIN.id, "3. low"}
    , {VariableAbstract::TYPE_VALUE_CLOSE.id, "4. close"}
    , {VariableAbstract::TYPE_VALUE_VOLUME.id, "5. volume"}
};

QMultiHash<QString, VariableAvailability>
StreamReaderAalphavantageStock::availableVariables() const
{
    QMultiHash<QString, VariableAvailability> availableVars;
    QStringList stockSymbols{"AAPL","MSFT","AMZN","GOOGL","GOOG","TSLA"
                             ,"BRK.B","NVDA","META","UNH","JNJ","V","WMT"
                             ,"XOM","JPM","PG","MA","HD","CVX","LLY","PFE"
                             ,"ABBV","MRK","PEP","KO","AVGO","COST","TMO"
                             ,"DIS","CSCO","ABT","DHR","MCD","ACN","ADBE"
                             ,"NFLX","CRM","BAC","WFC","TXN","QCOM","CMCSA"
                             ,"NKE","VZ","LIN","ORCL","PM","INTC","LOW","UPS"
                             ,"AMD","SBUX","UNP","RTX","HON","SPGI","INTU"
                             ,"GS","BLK","AXP","MDT","T","DE","CAT","IBM"
                             ,"GE","MMM","FIS","ISRG","PYPL","NOW","ADI"
                             ,"BKNG","GILD","AMT","PLD","LRCX","TJX","CHTR"
                             ,"DUK","SO","NEE","D","CCI","REGN","EL","CL"
                             ,"MO","APD","BDX","ZTS","SYK","AON","SHW"
                             ,"ECL","ITW","ETN","WM","NSC","FDX","EMR"
                             ,"BSX","KLAC","ADP","ROP","ICE","VRTX","ANET"
                             ,"ADS.DE","ALV.DE","ASML.AS","AIR.PA","AI.PA"
                             ,"ABI.BR","ABBN.SW","AD.AS","AENA.MC","AGN.AS"
                             ,"AIXA.MC","ALM.MC","AMS.MC","ANA.MC","BBVA.MC"
                             ,"BAYN.DE","BMW.DE","BNP.PA","BP.L","CS.PA","CRG.IR"
                             ,"DAI.DE","DG.PA","DTE.DE","ENEL.MI","ENI.MI","ELE.MC"
                             ,"EL.PA","ENGI.PA","EOAN.DE","ERIC.B.ST","ESSITYB.ST"
                             ,"FME.DE","FP.PA","FR.PA","G.MI","GSK.L","HEIO.AS"
                             ,"HLAG.DE","HSBA.L","IBE.MC","IFX.DE","INGA.AS"
                             ,"ITX.MC","KER.PA","KNEBV.HE","LHA.DE","LIN.DE"
                             ,"LLOY.L","LONN.SW","MC.PA","MUV2.DE","NOKIA.HE"
                             ,"ORA.PA","PHIA.AS","PRX.AS","REE.MC","REP.MC"
                             ,"RI.PA","RNO.PA","RWE.DE","SAN.MC","SAN.PA"
                             ,"SAP.DE","SAF.PA","SGO.PA","SIE.DE","SU.PA"
                             ,"SWEDA.ST","TEF.MC","TIT.MI","TTE.PA","UBSG.SW"
                             ,"ULVR.L","UNA.AS","VIV.PA","VOW3.DE","VK.PA","ZURN.SW"
                             ,"SHOP.TO","TD.TO","RY.TO","CNR.TO","CP.TO","ENB.TO"
                             ,"BMO.TO","BNS.TO","TRI.TO","SU.TO","CNQ.TO","ATD.TO"
                             ,"MFC.TO","WCN.TO","TFII.TO","L.TO","NTR.TO","POW.TO"
                             ,"CSU.TO","AQN.TO","BHP.AX","CBA.AX","CSL.AX","RIO.AX"
                             ,"NAB.AX","WBC.AX","ANZ.AX","WES.AX","FMG.AX","TLS.AX"
                             ,"MQG.AX","WOW.AX","COL.AX","ALL.AX","AMC.AX","BXB.AX"
                             ,"QAN.AX","ORG.AX","TCL.AX","WPL.AX","7203.T","9984.T"
                             ,"6861.T","9432.T","8306.T","9983.T","8035.T","4063.T"
                             ,"6954.T","7733.T","4502.T","4519.T","6501.T","6758.T"
                             ,"7267.T","7751.T","7974.T","8001.T","8031.T","8766.T"
                             ,"005930.KS","000660.KS","035420.KS","051910.KS","006400.KS"
                             ,"035720.KS","207940.KS","066570.KS","034730.KS","012330.KS"
                             ,"028260.KS","086790.KS","032830.KS","055550.KS","017670.KS"
                             ,"105560.KS","036570.KS","011200.KS","000270.KS","030200.KS"
                             ,"NOVN.SW","ROG.SW","NESN.SW","UBSG.SW","CSGN.SW","ZURN.SW"
                             ,"ABBN.SW","SGSN.SW","LONN.SW","GIVN.SW","SREN.SW","SCMN.SW"
                             ,"BAER.SW","CFR.SW","GEBN.SW","SIX.SW","ALC.SW","LOGN.SW"
                             ,"TEMN.SW","VIFN.SW","NHY.OL","EQNR.OL","DNB.OL","TEL.OL"
                             ,"ORK.OL","YAR.OL","MOWI.OL","SUBC.OL","AUSS.OL","GJF.OL"
                             ,"AKERBP.OL","STOR.OL","SATS.OL","REC.OL","KOG.OL","BAKKA.OL"
                             ,"NOD.OL","OBX.OL","TGS.OL","PGS.OL","AIR.NZ","ANZ.NZ","ATM.NZ"
                             ,"FPH.NZ","FRE.NZ","GMT.NZ","MFT.NZ","RYM.NZ","SPK.NZ","VHP.NZ"
                             ,"WBC.NZ","ZEL.NZ","CEN.NZ","EBO.NZ","KMD.NZ","MCY.NZ","NZX.NZ"
                             ,"SKC.NZ","SKL.NZ","VCT.NZ"};
    for (const auto &symbol : stockSymbols)
    {
        VariableAvailability variableAvailable;
        variableAvailable.tickIds = {Tick::TICK_MIN_1.id()
                                     , Tick::TICK_MIN_5.id()
                                     , Tick::TICK_MIN_15.id()
                                     , Tick::TICK_MIN_30.id()
                                     , Tick::TICK_HOUR_1.id()
                                     , Tick::TICK_DAY_1.id()
        };
        variableAvailable.variable = new Stock{symbol, "USD"};
        variableAvailable.streamReader = this;
        availableVars.insert(variableAvailable.variable->name(), variableAvailable);
    }
    return availableVars;
}

QTimeZone StreamReaderAalphavantageStock::timeZone() const
{
    return QTimeZone("America/New_York");
}

void StreamReaderAalphavantageStock::readData(
    const SortedMap<QString, QVariant> &params,
    const Tick &tick,
    VariableAbstract *variable,
    const QDate &dateFrom,
    const QDate &dateTo,
    QSharedPointer<Job> job) const
{
    _clearFinishedFutures();

    QList<QPair<QDate, QDate>> dateRange;
    QString apiKey = params[PARAM_API_KEY].toString();
    int maxQueryPerDay = params[PARAM_MAX_PER_DAY].toInt();
    int queryElapsedMs = params[PARAM_ELASPED_MS].toInt();
    auto stock = static_cast<Stock *>(variable);
    QSettings settings;
    job->start();
    const QList<QDateTime> &dateTimeMissing
        = variable->readDateTimeMissing(
            dateFrom, dateTo, tick);
    QSet<QDateTime> dateTimeMissingSet{dateTimeMissing.begin(), dateTimeMissing.end()};
    const QList<QDate> &months
        = variable->readMonthsMissing(
            dateTimeMissing);

    // Launch the asynchronous task
    QSharedPointer<std::future<void>> future(new std::future<void>);
    *future = std::move(std::async(std::launch::async, [this, maxQueryPerDay, queryElapsedMs, job, tick, stock, dateTimeMissingSet, months, apiKey]() {
        QNetworkAccessManager manager;

        // Generate the list of months to fetch data for
        static QMutex mutex;
        QMutexLocker mutexLocker{&mutex};
        for (const QDate &month : months)
        {
            // Format the month parameter (YYYY-MM)
            QString monthParam = month.toString("yyyy-MM");

            // Construct the API URL with required and optional parameters
            auto tempHash = HASH_TICK;
            auto tempId = tick.id();
            Q_ASSERT(HASH_TICK.contains(tick.id()));
            QString interval = HASH_TICK[tick.id()];
            QUrl url("https://www.alphavantage.co/query");
            QUrlQuery q;
            q.addQueryItem("function",   "TIME_SERIES_INTRADAY");
            q.addQueryItem("symbol",     stock->symbol());
            q.addQueryItem("interval",   interval);                   // ← must set this
            q.addQueryItem("outputsize", "full");
            q.addQueryItem("apikey",     apiKey);
            q.addQueryItem("adjusted",   "true");
            q.addQueryItem("month",   monthParam);
            q.addQueryItem("extended_hours",   "true");
            // "extended_hours" isn’t supported on the free TIME_SERIES_INTRADAY endpoint—drop it
            url.setQuery(q);

            // --- Retry loop: If we hit a rate-limit (detected via "Note"), sleep 10 seconds and try again ---
            bool querySuccessful = false;
            int retryCount = 0;
            QJsonObject jsonObject;
            qint64 msToWait = 0;
            while (!querySuccessful && retryCount < 10) // you can adjust the maximum retries if desired
            {
                QString keyLastDayQuery = "StreamReaderAalphavantageStockLastDay";
                QString keyNumberQuery = "StreamReaderAalphavantageStockNumberQuery";
                QDateTime lastDateTime = getSettingsValue(
                                                     keyLastDayQuery,
                                                     currentDateTime().addDays(-1)).toDateTime();
                int nQueriesToday = 0;
                const auto &current = currentDateTime();
                if (lastDateTime.daysTo(current) == 0)
                {
                    nQueriesToday = getSettingsValue(keyNumberQuery, 0).toInt();
                }
                qDebug() << "nQueriesToday:" << nQueriesToday;
                if (maxQueryPerDay > 0 && maxQueryPerDay == nQueriesToday)
                {
                    auto tomorrow = current.addDays(1);
                    tomorrow.setTime(QTime{0, 0, 0});
                    msToWait = current.msecsTo(tomorrow);
                }
                else if (queryElapsedMs > 0)
                {
                    msToWait = queryElapsedMs - lastDateTime.msecsTo(current);
                    if (msToWait < 0)
                    {
                        msToWait = 0;
                    }
                }
                if (!job->isRunning())
                {
                    return;
                }
                else if (msToWait > 0)
                {
                    qDebug() << "Waiting for" << msToWait / 1000 << "seconds";
                    int intervalMs = 1000;
                    for (qint64 i = 0; i<msToWait; i+= intervalMs)
                    {
                        QThread::msleep(qMin(intervalMs, msToWait - i*intervalMs));
                        if (!job->isRunning())
                        {
                            stock->closeDatabaseOpened(tick.id());
                            return;
                        }
                    }
                }
                // Create a new event loop for this attempt
                QEventLoop loop;
                QObject::connect(&manager, &QNetworkAccessManager::finished, &loop, &QEventLoop::quit);

                // Send the GET request
                qDebug() << "URL alphavantage:" << url;
                QNetworkReply *reply = manager.get(QNetworkRequest(QUrl(url)));
                //++nQueriesToday; //TODO I stop here

                // Wait for the network response
                loop.exec();

                setSettingsValue(keyLastDayQuery, QDateTime::currentDateTime());
                setSettingsValue(keyNumberQuery, getSettingsValue(keyNumberQuery, 0).toInt() + 1);

                // Handle network errors
                if (reply->error() != QNetworkReply::NoError)
                {
                    qWarning() << "Network error while fetching data for symbol"
                               << stock->symbol()
                               << "and month"
                               << monthParam
                               << ":"
                               << reply->errorString();
                    reply->deleteLater();
                    ++retryCount;
                    continue;
                }

                // Read and parse the response data

                QByteArray responseData = reply->readAll();
                reply->deleteLater();

                QJsonDocument jsonResponse = QJsonDocument::fromJson(responseData);
                if (jsonResponse.isNull() || !jsonResponse.isObject())
                {
                    qWarning() << "Invalid JSON response for symbol"
                               << stock->symbol()
                               << "and month"
                               << monthParam;
                    qWarning() << responseData;
                    QThread::sleep(60);
                    ++retryCount;
                    continue;
                }

                jsonObject = jsonResponse.object();

                // Check for API errors (other than rate limiting)
                if (jsonObject.contains("Error Message"))
                {
                    qWarning() << "API Error for symbol"
                               << stock->symbol()
                               << "and month"
                               << monthParam
                               << ":"
                               << jsonObject["Error Message"].toString();
                    QThread::sleep(60);
                    ++retryCount;
                    break; // break out of retry loop on API errors that are not rate-limit
                }

                // Check if the API is returning a note (e.g. rate limit reached)
                if (jsonObject.contains("Note"))
                {
                    qWarning() << "API Note (rate limit reached) for symbol"
                               << stock->symbol()
                               << "and month"
                               << monthParam
                               << ":"
                               << jsonObject["Note"].toString();
                    QThread::sleep(10); // sleep 10 seconds before retrying
                    ++retryCount;
                    continue;
                }

                // If we reached here, the query appears successful
                querySuccessful = true;
            } // End retry loop

            // If the query was not successful after retries, move on to the next month
            if (!querySuccessful)
            {
                continue;
            }

            // --- Process the successful response ---
            QString timeSeriesKey = QString("Time Series (%1)").arg(interval);
            if (!jsonObject.contains(timeSeriesKey))
            {
                qWarning() << "Time series data not found for symbol"
                           << stock->symbol()
                           << "and month"
                           << monthParam;
                QThread::sleep(60);
                continue;
            }

            QJsonObject timeSeries = jsonObject[timeSeriesKey].toObject();
            QList<QDateTime> dateTimeMissingSetsTEMP{dateTimeMissingSet.begin(), dateTimeMissingSet.end()};

            // Iterate over each data point in the time series
            for (auto it = timeSeries.begin(); it != timeSeries.end(); ++it)
            {
                QString dateTimeStr = it.key();
                QDateTime dateTime = QDateTime::fromString(dateTimeStr, "yyyy-MM-dd HH:mm:ss");
                dateTime.setTimeZone(timeZone());

                QDate date = dateTime.date();
                QDateTime dateTimeDebug(date, dateTime.time());

                // Filter data points within the specified date range
                if (!dateTimeMissingSet.contains(dateTime)) //TODO I stop here
                {
                    for (const auto &dateTimeTEMP : dateTimeMissingSetsTEMP)
                    {
                        if (qAbs(dateTimeTEMP.secsTo(dateTimeDebug)) < 60*59)
                        {
                            int TEMP=10;++TEMP;
                        }
                    }
                    continue; // Skip data points outside the date range
                }

                QJsonObject ohlcv = it.value().toObject();

                // Extract OHLCV values using HASH_VALUE_TYPES
                QStringList valueTypeIds;
                QList<double> stockValues;
                for (auto hashIt = HASH_VALUE_TYPES.begin(); hashIt != HASH_VALUE_TYPES.end(); ++hashIt)
                {
                    valueTypeIds << hashIt.key();
                    stockValues << ohlcv[hashIt.value()].toString().toDouble();
                }

                // Insert each OHLCV value into the database
                for (int i = 0; i < valueTypeIds.size(); ++i)
                {
                    QString typeValueId = valueTypeIds.at(i); //TODO Max and Close are not saved + check if date time not done is correct
                    double value = stockValues.at(i);
                    stock->recordInDatabase(dateTime, tick, typeValueId, value);
                }
            }

        }
        stock->closeDatabaseOpened(tick.id());

        qDebug() << "Completed fetching historical data for symbol" << stock->symbol();
    }));

    // Store the future to manage its lifecycle
    m_futures << future;
}

void StreamReaderAalphavantageStock::_clearFinishedFutures() const
{
    for (int i=m_futures.size()-1; i>= 0; --i)
    {
        auto status = m_futures[i]->wait_for(std::chrono::seconds(0)); // Zero duration: non-blocking check
        if (status == std::future_status::ready)
        {
            m_futures.remove(i);
        }
    }
}


