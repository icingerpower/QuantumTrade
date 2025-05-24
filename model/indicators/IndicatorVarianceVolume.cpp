#include "IndicatorVarianceVolume.h"

RECORD_INDICATOR(IndicatorVarianceVolume);

QString IndicatorVarianceVolume::id() const
{
    return "IndicatorVarianceVolume";
}

QString IndicatorVarianceVolume::name() const
{
    return QObject::tr("Volume var");
}

QString IndicatorVarianceVolume::description() const
{
    return QObject::tr("Variance of the volume");
}

double IndicatorVarianceVolume::compute(
        std::deque<std::vector<double>> &queueOfValues,
        int colIndexLow,
        int colIndexHigh,
        int colIndexOpen,
        int colIndexClose,
        int colIndexVolume,
        const Tick *,
        const QMap<QString, QVariant> &params) const
{
    double avg = 0.;
    const int sizeSample = qMin(int(queueOfValues.size()),
                                params.value(PAR_ID_SIZE_SAMPLE).toInt());
    struct Acc { double sum = 0., sum2 = 0.; } acc;
    std::for_each(
        queueOfValues.begin(),
        queueOfValues.begin() + sizeSample,
        [&](auto& row) {
            double x = row[colIndexVolume];
            acc.sum  += x;
            acc.sum2 += x * x;
        }
        );

    double mean = acc.sum / sizeSample;
    return (acc.sum2 / sizeSample) - (mean * mean);
}

