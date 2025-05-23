#include "IndicatorOBV.h"
#include <limits>

RECORD_INDICATOR(IndicatorOBV);

QString IndicatorOBV::name() const
{
    return QObject::tr("On-Balance Volume");
}

QString IndicatorOBV::description() const
{
    return QObject::tr("Cumulative volume flow: +vol on up-days, –vol on down-days");
}

double IndicatorOBV::compute(
    std::deque<std::vector<double>>& queueOfValues,
    int,
    int colIndexVolume,
    int,
    int colIndexClose,
    const Tick*,
    const QMap<QString, QVariant>& params) const
{
    // Determine how many bars we actually have
    int sizeSample = qMin(int(queueOfValues.size()),
                          params.value(PAR_ID_SIZE_SAMPLE).toInt());
    // Need at least two bars to compare closes
    if (sizeSample < 2)
    {
        return std::numeric_limits<double>::quiet_NaN();
    }

    double obv = 0.0;
    // Start at 1 so we can compare to the previous close
    for (int i = 1; i < sizeSample; ++i)
    {
        double todayClose = queueOfValues[i][colIndexClose];
        double prevClose  = queueOfValues[i - 1][colIndexClose];
        double vol        = queueOfValues[i][colIndexVolume];
        if (todayClose > prevClose)
        {
            obv += vol;
        }
        else if (todayClose < prevClose)
        {
            obv -= vol;
        }
        // if equal, OBV unchanged
    }

    return obv;
}

QList<QMap<QString, QVariant>> IndicatorOBV::possibleParams() const
{
    return {
        // Short-term OBV (last 100 bars)
        { { PAR_ID_SIZE_SAMPLE, 100 } },
        // Medium-term OBV (last 250 bars)
        { { PAR_ID_SIZE_SAMPLE, 250 } },
        // Long-term OBV (last 500 bars)
        { { PAR_ID_SIZE_SAMPLE, 500 } }
    };
}
