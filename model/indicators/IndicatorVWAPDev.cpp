#include <limits>

#include "IndicatorVWAPDev.h"

RECORD_INDICATOR(IndicatorVWAPDev);

QString IndicatorVWAPDev::id() const
{
    return "IndicatorVWAPDev";
}

QString IndicatorVWAPDev::name() const
{
    return QObject::tr("VWAP Deviation");
}

QString IndicatorVWAPDev::description() const
{
    return QObject::tr("Percent deviation of latest price from VWAP over N bars");
}

double IndicatorVWAPDev::compute(
        std::deque<std::vector<double>>& queueOfValues,
        int colIndexLow,
        int colIndexHigh,
        int colIndexOpen,
        int colIndexClose,
        int colIndexVolume,
        const Tick*,
        const QMap<QString, QVariant>& params) const
{
    // Determine how many bars we actually have
    int sizeSample = qMin(int(queueOfValues.size()),
                          params.value(PAR_ID_SIZE_SAMPLE).toInt());
    if (sizeSample < 1)
    {
        return std::numeric_limits<double>::quiet_NaN();
    }

    // Compute VWAP = sum(price*vol) / sum(vol)
    double sumPV = 0.0;
    double sumV  = 0.0;
    for (int i = 0; i < sizeSample; ++i)
    {
        double price = queueOfValues[i][colIndexClose];
        double vol   = queueOfValues[i][colIndexVolume];
        sumPV += price * vol;
        sumV  += vol;
    }
    if (sumV == 0.0)
    {
        return std::numeric_limits<double>::quiet_NaN();
    }

    double vwap = sumPV / sumV;
    double lastPrice = queueOfValues[sizeSample - 1][colIndexClose];

    // return percentage deviation (lastPrice - VWAP) / VWAP
    return (lastPrice - vwap) / vwap;
}

QList<QMap<QString, QVariant>> IndicatorVWAPDev::possibleParams() const
{
    return {
        // fairly short-term VWAP deviation
        { { PAR_ID_SIZE_SAMPLE,  50 } },
        // medium-term
        { { PAR_ID_SIZE_SAMPLE, 100 } },
        // longer-term
        { { PAR_ID_SIZE_SAMPLE, 200 } }
    };
}
