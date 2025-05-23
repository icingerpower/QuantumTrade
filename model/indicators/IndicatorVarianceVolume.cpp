#include "IndicatorVarianceVolume.h"

RECORD_INDICATOR(IndicatorVarianceVolume);

QString IndicatorVarianceVolume::name() const
{
    return "VarianceVolume";
}

QString IndicatorVarianceVolume::description() const
{
    return "VarianceVolume";
}

double IndicatorVarianceVolume::compute(
    std::deque<std::vector<double>> &queueOfValues,
    int,
    int colIndexVolume,
    int,
    int,
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

