#include "IndicatorStoch.h"
#include <algorithm>
#include <limits>
#include <cmath>
#include <QObject>

RECORD_INDICATOR(IndicatorStoch);

// static member definitions
QString IndicatorStoch::PAR_ID_K_PERIOD{"kPeriod"};
QPair<QString, QString> IndicatorStoch::PAR_ID_NAME_K_PERIOD{
    IndicatorStoch::PAR_ID_K_PERIOD,
    QObject::tr("%K Look‐back Period")
};
QString IndicatorStoch::PAR_ID_D_PERIOD{"dPeriod"};
QPair<QString, QString> IndicatorStoch::PAR_ID_NAME_D_PERIOD{
    IndicatorStoch::PAR_ID_D_PERIOD,
    QObject::tr("%D Smoothing Period")
};

QString IndicatorStoch::name() const
{
    return QObject::tr("Stoch %K–%D Histogram");
}

QString IndicatorStoch::description() const
{
    return QObject::tr("Difference between %K and its %D moving average");
}

double IndicatorStoch::compute(
    std::deque<std::vector<double>>& q,
    int colIndexValue,
    int, int, int,
    const Tick*,
    const QMap<QString, QVariant>& params) const
{
    int kPeriod = params.value(PAR_ID_K_PERIOD).toInt();
    int dPeriod = params.value(PAR_ID_D_PERIOD).toInt();
    int bufferSize = params.value(PAR_ID_SIZE_SAMPLE).toInt();

    // how many bars we actually have
    int sizeSample = qMin(int(q.size()), bufferSize);
    // we need at least kPeriod bars to compute one %K,
    // plus (dPeriod−1) more to average into %D
    if (sizeSample < kPeriod + dPeriod - 1)
    {
        return std::numeric_limits<double>::quiet_NaN();
    }

    // extract just the price series
    std::vector<double> prices(sizeSample);
    for (int i = 0; i < sizeSample; ++i)
    {
        prices[i] = q[i][colIndexValue];
    }

    // compute %K series: (price – lowestLow)/(highestHigh – lowestLow) * 100
    int kCount = sizeSample - kPeriod + 1;
    std::vector<double> kSeries;
    kSeries.reserve(kCount);
    for (int start = 0; start < kCount; ++start)
    {
        auto first = prices.begin() + start;
        auto last  = first + kPeriod;
        double low  = *std::min_element(first, last);
        double high = *std::max_element(first, last);
        double curr = prices[start + kPeriod - 1];
        double kVal = (high > low)
                          ? ((curr - low) / (high - low)) * 100.0
                          : 0.0;
        kSeries.push_back(kVal);
    }

    // compute %D as simple MA of the last dPeriod %K values
    double sumD = 0.0;
    for (int i = kCount - dPeriod; i < kCount; ++i)
    {
        sumD += kSeries[i];
    }
    double dVal = sumD / dPeriod;

    // histogram: last %K minus %D
    double lastK = kSeries.back();
    return lastK - dVal;
}

QList<QMap<QString, QVariant>> IndicatorStoch::possibleParams() const
{
    return {
        // classic 14,3 stochastic
        {
            { PAR_ID_SIZE_SAMPLE, 17 },  // need kPeriod+dPeriod-1 = 14+3-1
            { PAR_ID_K_PERIOD,       14 },
            { PAR_ID_D_PERIOD,        3 }
        },
        // faster 5,3
        {
            { PAR_ID_SIZE_SAMPLE,  7 },  // 5+3-1
            { PAR_ID_K_PERIOD,       5 },
            { PAR_ID_D_PERIOD,       3 }
        },
        // slower 21,5
        {
            { PAR_ID_SIZE_SAMPLE, 25 },  // 21+5-1
            { PAR_ID_K_PERIOD,      21 },
            { PAR_ID_D_PERIOD,       5 }
        }
    };
}
