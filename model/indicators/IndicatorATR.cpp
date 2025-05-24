#include "IndicatorATR.h"
#include <limits>

RECORD_INDICATOR(IndicatorATR);

QString IndicatorATR::id() const
{
    return "IndicatorATR";
}

QString IndicatorATR::name() const
{
    return QObject::tr("ATR (close-to-close)");
}

QString IndicatorATR::description() const
{
    return QObject::tr("Average of absolute close-to-close moves over N bars");
}

double IndicatorATR::compute(
        std::deque<std::vector<double>>& queueOfValues,
        int,
        int,
        int,
        int colIndexClose,
        int,
        const Tick*,
        const QMap<QString, QVariant>& params) const
{
    // buffer size = N+1 (we need one extra bar to form N deltas)
    int sizeSample = qMin(int(queueOfValues.size()),
                          params.value(PAR_ID_SIZE_SAMPLE).toInt());
    int period = sizeSample - 1;
    if (period < 1)
        return std::numeric_limits<double>::quiet_NaN();

    double sumAbs = 0.0;
    // accumulate absolute changes: |close[i] - close[i-1]|
    for (int i = 1; i < sizeSample; ++i) {
        double today = queueOfValues[i][colIndexClose];
        double prev  = queueOfValues[i - 1][colIndexClose];
        sumAbs += std::abs(today - prev);
    }

    return sumAbs / period;
}

QList<QMap<QString, QVariant>> IndicatorATR::possibleParams() const
{
    return {
        { { PAR_ID_SIZE_SAMPLE, 15 } },  // classic 14-period ATR → need 15 closes
        { { PAR_ID_SIZE_SAMPLE, 8  } },  // faster 7-period
        { { PAR_ID_SIZE_SAMPLE, 29 } }   // slower 28-period
    };
}
