#include "IndicatorMACD.h"
#include <limits>
#include <cmath>
#include <QObject>

RECORD_INDICATOR(IndicatorMACD);

// static member definitions
QString IndicatorMACD::PAR_ID_FAST_PERIOD{"fastPeriod"};
QPair<QString, QString> IndicatorMACD::PAR_ID_NAME_FAST_PERIOD{
    IndicatorMACD::PAR_ID_FAST_PERIOD,
    QObject::tr("Fast EMA Period")
};
QString IndicatorMACD::PAR_ID_SLOW_PERIOD{"slowPeriod"};
QPair<QString, QString> IndicatorMACD::PAR_ID_NAME_SLOW_PERIOD{
    IndicatorMACD::PAR_ID_SLOW_PERIOD,
    QObject::tr("Slow EMA Period")
};
QString IndicatorMACD::PAR_ID_SIGNAL_PERIOD{"signalPeriod"};
QPair<QString, QString> IndicatorMACD::PAR_ID_NAME_SIGNAL_PERIOD{
    IndicatorMACD::PAR_ID_SIGNAL_PERIOD,
    QObject::tr("Signal EMA Period")
};

QString IndicatorMACD::name() const
{
    return QObject::tr("MACD Histogram");
}

QString IndicatorMACD::description() const
{
    return QObject::tr("Difference between MACD line and its signal line");
}

double IndicatorMACD::compute(
    std::deque<std::vector<double>>& queueOfValues,
    int colIndexValue,
    int, int, int,
    const Tick*,
    const QMap<QString, QVariant>& params) const
{
    int fastPeriod   = params.value(PAR_ID_FAST_PERIOD).toInt();
    int slowPeriod   = params.value(PAR_ID_SLOW_PERIOD).toInt();
    int signalPeriod = params.value(PAR_ID_SIGNAL_PERIOD).toInt();
    int bufferSize   = params.value(PAR_ID_SIZE_SAMPLE).toInt();

    int sizeSample = qMin(int(queueOfValues.size()), bufferSize);
    if (sizeSample < slowPeriod + signalPeriod)
    {
        return std::numeric_limits<double>::quiet_NaN();
    }

    // copy relevant prices
    std::vector<double> prices(sizeSample);
    for (int i = 0; i < sizeSample; ++i)
    {
        prices[i] = queueOfValues[i][colIndexValue];
    }

    // EMA helper
    auto calcEMA = [&](int period, const std::vector<double>& src)
    {
        double alpha = 2.0 / (period + 1.0);
        double ema = src.front();
        for (size_t i = 1; i < src.size(); ++i)
            ema = alpha * src[i] + (1 - alpha) * ema;
        return ema;
    };

    // build MACD series
    std::vector<double> macdSeries;
    macdSeries.reserve(sizeSample - slowPeriod + 1);
    for (int offset = 0; offset <= sizeSample - slowPeriod; ++offset)
    {
        // slow EMA over window
        std::vector<double> window(
            prices.begin() + offset,
            prices.begin() + offset + slowPeriod
            );
        double slowE = calcEMA(slowPeriod, window);

        // fast EMA over its tail
        std::vector<double> fastWindow(
            window.begin() + (slowPeriod - fastPeriod),
            window.end()
            );
        double fastE = calcEMA(fastPeriod, fastWindow);

        macdSeries.push_back(fastE - slowE);
    }

    // signal line over MACD
    double signal = calcEMA(signalPeriod, macdSeries);
    double lastMacd = macdSeries.back();
    return lastMacd - signal;
}

QList<QMap<QString, QVariant>> IndicatorMACD::possibleParams() const
{
    return {
        {
            { PAR_ID_SIZE_SAMPLE,      1000 },
            { PAR_ID_FAST_PERIOD,       12 },
            { PAR_ID_SLOW_PERIOD,       26 },
            { PAR_ID_SIGNAL_PERIOD,      9 }
        },
        {
            { PAR_ID_SIZE_SAMPLE,       500 },
            { PAR_ID_FAST_PERIOD,        5 },
            { PAR_ID_SLOW_PERIOD,       35 },
            { PAR_ID_SIGNAL_PERIOD,      5 }
        },
        {
            { PAR_ID_SIZE_SAMPLE,       800 },
            { PAR_ID_FAST_PERIOD,       10 },
            { PAR_ID_SLOW_PERIOD,       30 },
            { PAR_ID_SIGNAL_PERIOD,     10 }
        }
    };
}
