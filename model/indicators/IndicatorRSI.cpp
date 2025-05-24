#include "IndicatorRSI.h"
#include <limits>
#include <QObject>

RECORD_INDICATOR(IndicatorRSI);

QString IndicatorRSI::id() const
{
    return "IndicatorRSI";
}

QString IndicatorRSI::name() const
{
    return QObject::tr("RSI");
}

QString IndicatorRSI::description() const
{
    return QObject::tr("Relative Strength Index");
}

double IndicatorRSI::compute(
    std::deque<std::vector<double>>& queueOfValues,
    int /*colIndexLow*/,
    int /*colIndexHigh*/,
    int /*colIndexOpen*/,
    int colIndexClose,
    int /*colIndexVolume*/,
    const Tick*,
    const QMap<QString, QVariant>& params) const
{
    // Use sizeSample directly as (period + 1) bars → period = sizeSample - 1
    int sizeSample = qMin(int(queueOfValues.size()),
                          params.value(PAR_ID_SIZE_SAMPLE).toInt());
    int period = sizeSample - 1;

    // Need at least two bars to compute a single delta
    if (period < 1)
        return std::numeric_limits<double>::quiet_NaN();

    double sumGain = 0.0, sumLoss = 0.0;
    // Start at i=1 to compute queue[i] - queue[i-1]
    for (int i = 1; i < sizeSample; ++i)
    {
        double delta = queueOfValues[i][colIndexClose]
                     - queueOfValues[i - 1][colIndexClose];
        if (delta > 0)
        {
            sumGain += delta;
        }
        else
        {
            sumLoss += -delta;
        }
    }

    double avgGain = sumGain / period;
    double avgLoss = sumLoss / period;

    // If no losses, RSI = 100; if no gains, RSI = 0
    if (avgLoss == 0.0) return 100.0;
    if (avgGain == 0.0) return 0.0;

    double rs = avgGain / avgLoss;
    return 100.0 - (100.0 / (1.0 + rs));
}

QList<QMap<QString, QVariant>> IndicatorRSI::possibleParams() const
{
    return {
        // RSI-14: need 15 bars to get 14 deltas
        { { PAR_ID_SIZE_SAMPLE, 15 } },
        // RSI-7
        { { PAR_ID_SIZE_SAMPLE, 8  } },
        // RSI-28
        { { PAR_ID_SIZE_SAMPLE, 29 } }
    };
}
