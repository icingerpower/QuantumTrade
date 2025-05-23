#include "IndicatorVariance.h"

RECORD_INDICATOR(IndicatorVariance);

QString IndicatorVariance::name() const
{
    return QObject::tr("Var");
}

QString IndicatorVariance::description() const
{
    return QObject::tr("Variance");
}

double IndicatorVariance::compute(
    std::deque<std::vector<double>> &queueOfValues,
    int colIndexValue,
    int,
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
            double x = row[colIndexValue];
            acc.sum  += x;
            acc.sum2 += x * x;
        }
        );

    double mean = acc.sum / sizeSample;
    return (acc.sum2 / sizeSample) - (mean * mean);
}

QList<QMap<QString, QVariant>> IndicatorVariance::possibleParams() const
{
    QList<QMap<QString, QVariant>> params;
    params << QMap<QString, QVariant>{{PAR_ID_SIZE_SAMPLE, 20}};
    params << QMap<QString, QVariant>{{PAR_ID_SIZE_SAMPLE, 100}};
    params << QMap<QString, QVariant>{{PAR_ID_SIZE_SAMPLE, 250}};
    return params;
}
