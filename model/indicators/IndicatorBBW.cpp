#include "IndicatorBBW.h"
#include <algorithm>
#include <cmath>
#include <limits>
#include <QObject>

RECORD_INDICATOR(IndicatorBBW);

// static member definitions
QString IndicatorBBW::PAR_ID_STDDEV_MULTIPLIER{"stddevMultiplier"};
QPair<QString, QString> IndicatorBBW::PAR_ID_NAME_STDDEV_MULTIPLIER{
    IndicatorBBW::PAR_ID_STDDEV_MULTIPLIER,
    QObject::tr("Std-Dev Multiplier")
};

QString IndicatorBBW::id() const
{
    return "IndicatorBBW";
}

QString IndicatorBBW::name() const
{
    return QObject::tr("Bollinger Band Width");
}

QString IndicatorBBW::description() const
{
    return QObject::tr("Width of the Bollinger Bands relative to the moving average");
}

double IndicatorBBW::compute(
    std::deque<std::vector<double>>& queueOfValues,
    int /*colIndexLow*/,
    int /*colIndexHigh*/,
    int /*colIndexOpen*/,
    int colIndexClose,
    int /*colIndexVolume*/,
    const Tick*,
    const QMap<QString, QVariant>& params) const
{
    int bufferSize = params.value(PAR_ID_SIZE_SAMPLE).toInt();
    int sizeSample = qMin(int(queueOfValues.size()), bufferSize);
    double mult    = params.value(PAR_ID_STDDEV_MULTIPLIER).toDouble();

    if (sizeSample < 1 || mult <= 0.0)
        return std::numeric_limits<double>::quiet_NaN();

    struct Acc { double sum = 0.0, sum2 = 0.0; } acc;
    std::for_each(
        queueOfValues.begin(),
        queueOfValues.begin() + sizeSample,
        [&](auto& row) {
            double price = row[colIndexClose];
            acc.sum  += price;
            acc.sum2 += price * price;
        }
    );

    double mean = acc.sum / sizeSample;
    if (mean == 0.0)
        return std::numeric_limits<double>::quiet_NaN();

    double var = (acc.sum2 / sizeSample) - (mean * mean);
    double sd  = std::sqrt(std::max(0.0, var));

    // (Upper band – Lower band) / Middle band = (2 * mult * sd) / mean
    return (2.0 * mult * sd) / mean;
}

QList<QMap<QString, QVariant>> IndicatorBBW::possibleParams() const
{
    return {
        // Classic: 20-bar SMA, 2σ
        {
            { PAR_ID_SIZE_SAMPLE,          20 },
            { PAR_ID_STDDEV_MULTIPLIER,    2.0 }
        },
        // Faster BB: 10-bar, 2σ
        {
            { PAR_ID_SIZE_SAMPLE,          10 },
            { PAR_ID_STDDEV_MULTIPLIER,    2.0 }
        },
        // Tighter bands: 20-bar, 1.5σ
        {
            { PAR_ID_SIZE_SAMPLE,          20 },
            { PAR_ID_STDDEV_MULTIPLIER,    1.5 }
        }
    };
}
