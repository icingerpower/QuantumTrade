#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QEventLoop>
#include <QThread>
#include <QTimeZone>

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

SortedMap<QString, StreamReaderAalphavantageStock::Param> StreamReaderAalphavantageStock::paramsDefault() const
{
    SortedMap<QString, Param> params;
    params[PARAM_API_KEY] = PARAM_API_KEY_NAME_VALUE;
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
    , {VariableAbstract::TYPE_VALUE_CLOSE.id, "2. close"}
    , {VariableAbstract::TYPE_VALUE_MIN.id, "3. low"}
    , {VariableAbstract::TYPE_VALUE_MAX.id, "4. max"}
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
        availableVars.insert(variableAvailable.variable->name(), variableAvailable);
    }
    /*
    QStringList cryptoSymbols{"BTC","ETH","BNB","XRP","SOL","ADA","DOGE"
                              ,"TRX","TON","LINK","MATIC","DOT","WBTC","AVAX","SHIB"
                              ,"LTC","DAI","UNI","BCH","LEO","ATOM","XLM","OKB","ETC"
                              ,"INJ","XMR","IMX","APT","FIL","HBAR","RUNE","VET"
                              ,"ARB","NEAR","OP","STX","MKR","SEI","GRT","AAVE","ALGO"
                              ,"EGLD","QNT","THETA","XTZ","FLOW","SAND","EOS","KAS"
                              ,"RNDR","MANA","KCS","BSV","BGB","SNX","CRV","MINA","APE"
                              ,"CHZ","FTM","AXS","GALA","DYDX","COMP","LDO","NEO","GMX"
                              ,"KAVA","WLD","ROSE","ZEC","IOTA","USDP","FET","XDC","FXS"
                              ,"ENS","CFX","TRUMP","1INCH","JUP","OCEAN","RSR","SKL"
                              ,"ASTR","CVX","GLMR","DASH","PEPE","BONK","ZIL","YFI"
                              ,"ENJ","QTUM","ANKR","IOTX","CELO","BLUR","GNO","ONE"
                              ,"CKB","LUNC","USTC","FLR","CORE","KLAY","HNT","WAXP"
                              ,"NEXO","ZRX"};
    for (const auto &symbol : cryptoSymbols)
    {
        VariableAvailability variableAvailable;
        variableAvailable.tickIds = {Tick::TICK_MIN_1.id()
                                     , Tick::TICK_MIN_5.id()
                                     , Tick::TICK_MIN_15.id()
                                     , Tick::TICK_HOUR_1.id()
                                     , Tick::TICK_DAY_1.id()};
        variableAvailable.historicalData = false;
        variableAvailable.variable = new Crypto{symbol, "USD"};
        availableVars.insert(variableAvailable.variable->name(), variableAvailable);
        variableAvailable.tickIds = {Tick::TICK_DAY_1.id()};
        variableAvailable.historicalData = true;
        availableVars.insert(variableAvailable.variable->name(), variableAvailable);
    }
    QStringList currencySymbols{"EUR","JPY","GBP","AUD","CAD","CHF","CNY","HKD","NZD"
                                ,"SEK","KRW","SGD","NOK","MXN","INR","RUB","ZAR","TRY","BRL"};
//*/
    return availableVars;
}

QSharedPointer<Job> StreamReaderAalphavantageStock::readLatestData(
        const SortedMap<QString, QVariant> &params,
        const Tick &tick,
        QList<VariableAbstract *> variables,
        QSharedPointer<Job> job)
{
    _clearFinishedFutures();
    job->start();
    QString apiKey = params[PARAM_API_KEY].toString();
    QSharedPointer<std::future<void>> future(new std::future<void>);
    *future = std::move(std::async(std::launch::async, [this, tick, variables, job, apiKey]() {
        QNetworkAccessManager manager;

        while (job->isRunning())
        {
            QDateTime newLastDateTime;
            for (auto &variable : variables)
            {
                auto stock = static_cast<Stock *>(variable);
                // Fetch the last recorded date for the given tick
                QDateTime lastDateTime = variables.first()->readDateTimeEnd(tick);
                newLastDateTime = lastDateTime;

                // Make an API call to Alpha Vantage
                QEventLoop loop;
                QObject::connect(&manager, &QNetworkAccessManager::finished, &loop, &QEventLoop::quit);

                QString url = QString("https://www.alphavantage.co/query?function=TIME_SERIES_INTRADAY&symbol=%1&interval=%2&apikey=%3")
                                  .arg(stock->name(), HASH_TICK[tick.id()], apiKey);

                QNetworkReply *reply = manager.get(QNetworkRequest(QUrl(url)));
                loop.exec();

                if (reply->error() == QNetworkReply::NoError)
                {
                    QByteArray data = reply->readAll();
                    QJsonDocument jsonResponse = QJsonDocument::fromJson(data);
                    QJsonObject jsonObject = jsonResponse.object();
                    QJsonObject timeSeries = jsonObject["Time Series (5min)"].toObject();

                    for (auto it = timeSeries.begin(); it != timeSeries.end(); ++it)
                    {
                        QDateTime dateTime = QDateTime::fromString(it.key(), "yyyy-MM-dd HH:mm:ss");
                        if (dateTime > lastDateTime) {
                            newLastDateTime = qMin(dateTime, newLastDateTime);
                            QJsonObject values = it.value().toObject();
                            QStringList valueTypeIds;
                            QList<double> stockValues;
                            for (auto it = HASH_VALUE_TYPES.begin();
                                 it != HASH_VALUE_TYPES.end(); ++it)
                            {
                                valueTypeIds << it.key();
                                stockValues << values[it.value()].toString().toDouble();
                            }
                            variable->recordInDatabase(dateTime, tick, valueTypeIds, stockValues);
                        }
                    }
                }
                else
                {
                    // Handle error
                    qDebug() << "Error fetching data from Alpha Vantage:" << reply->errorString();
                }


                reply->deleteLater();
            }
            int secsToNow = newLastDateTime.secsTo(QDateTime::currentDateTime());
            int secsToWait = tick.seconds() - secsToNow + 3;
            if (secsToWait < 0)
            {
                secsToWait = 10;
            }

            // Sleep for the interval specified by tick.seconds() before the next fetch
            QThread::sleep(secsToWait); //We had 3 seconds of potential latence
        }
    }));
    m_futures << future;

    return job;
}

void StreamReaderAalphavantageStock::readHistoricalData(
        const SortedMap<QString, QVariant> &params,
        const Tick &tick,
        VariableAbstract *variable,
        const QDate &dateFrom,
        const QDate &dateTo)
{
    _clearFinishedFutures();

    QString apiKey = params[PARAM_API_KEY].toString();
    auto stock = static_cast<Stock *>(variable);

    // Launch the asynchronous task
    QSharedPointer<std::future<void>> future(new std::future<void>);
    *future = std::move(std::async(std::launch::async, [this, tick, stock, dateFrom, dateTo, apiKey]() {
        QNetworkAccessManager manager;

        // Generate the list of months to fetch data for
        QList<QDate> months = generateMonths(dateFrom, dateTo);

        for (const QDate &month : months)
        {
            // Format the month parameter (YYYY-MM)
            QString monthParam = month.toString("yyyy-MM");

            // Construct the API URL with required and optional parameters
            QString interval = HASH_TICK[tick.id()];
            QString url = QString("https://www.alphavantage.co/query?"
                                  "function=TIME_SERIES_INTRADAY&"
                                  "symbol=%1&"
                                  "interval=%2&"
                                  "outputsize=full&"
                                  "apikey=%3")
                          .arg(stock->symbol(), interval, apiKey);
            url += "&adjusted=true&extended_hours=true";

            // --- Retry loop: If we hit a rate-limit (detected via "Note"), sleep 10 seconds and try again ---
            bool querySuccessful = false;
            int retryCount = 0;
            QJsonObject jsonObject;
            while (!querySuccessful && retryCount < 100) // you can adjust the maximum retries if desired
            {
                if (!QThread::currentThread()->isInterruptionRequested())
                {
                    // Create a new event loop for this attempt
                    QEventLoop loop;
                    QObject::connect(&manager, &QNetworkAccessManager::finished, &loop, &QEventLoop::quit);

                    // Send the GET request
                    QNetworkReply *reply = manager.get(QNetworkRequest(QUrl(url)));

                    // Wait for the network response
                    loop.exec();

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
                        QThread::sleep(60); // longer sleep for network errors
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
                }
                else
                {
                    // If the thread has been interrupted, exit the retry loop.
                    break;
                }
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

            // Iterate over each data point in the time series
            for (auto it = timeSeries.begin(); it != timeSeries.end(); ++it)
            {
                QString dateTimeStr = it.key();
                QDateTime dateTime = QDateTime::fromString(dateTimeStr, "yyyy-MM-dd HH:mm:ss");
                dateTime.setTimeZone(QTimeZone::utc());

                QDate date = dateTime.date();

                // Filter data points within the specified date range
                if (date < dateFrom || date > dateTo)
                {
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
                    QString typeValueId = valueTypeIds.at(i);
                    double value = stockValues.at(i);
                    stock->recordInDatabase(dateTime, tick, typeValueId, value);
                }
            }

            // Respect API rate limits by introducing a delay between monthly queries
            QThread::sleep(15); // Sleep for 15 seconds between API calls
        }

        qDebug() << "Completed fetching historical data for symbol" << stock->symbol();
    }));

    // Store the future to manage its lifecycle
    m_futures << future;
}

QList<QDate> StreamReaderAalphavantageStock::generateMonths(
    const QDate &startDate, const QDate &endDate) const
{
    QList<QDate> months;
    QDate current = QDate(startDate.year(), startDate.month(), 1);
    QDate last = QDate(endDate.year(), endDate.month(), 1);
    while (current <= last)
    {
        months.append(current);
        current = current.addMonths(1);
    }
    return months;
}

void StreamReaderAalphavantageStock::readHistoricalData(
        const SortedMap<QString, QVariant> &params,
        const Tick &tick,
        QList<VariableAbstract *> variables,
        const QDate &dateFrom,
        const QDate &dateTo)
{
    // We do nothing because retreiving data in bulk is not available
}

void StreamReaderAalphavantageStock::_clearFinishedFutures()
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

