#include "IndicatorVarianceNegative.h"

RECORD_INDICATOR(IndicatorVarianceNegative);

QString IndicatorVarianceNegative::id() const
{
    return "IndicatorVarianceNegative";
}

QString IndicatorVarianceNegative::name() const
{
    return QObject::tr("Neg var");
}

QString IndicatorVarianceNegative::description() const
{
    return QObject::tr("Negative variance");
}

double IndicatorVarianceNegative::compute(
        std::deque<std::vector<double>> &queueOfValues,
        int,
        int,
        int,
        int colIndexClose,
        int,
        const Tick *,
        const QMap<QString, QVariant> &params) const
{
    double avg = 0.;
    const int sizeSample = qMin(int(queueOfValues.size()),
                                params.value(PAR_ID_SIZE_SAMPLE).toInt());
    for (int i=0; i<sizeSample; ++i)
    {
        avg += queueOfValues[i][colIndexClose];
    }
    avg /= sizeSample;
    double sumSquareDiff = 0.;
    int nDiff = 0;
    for (int i=0; i<sizeSample; ++i)
    {
        double diff = queueOfValues[i][colIndexClose] - avg;
        if (diff < 0)
        {
            sumSquareDiff += diff * diff;
            ++nDiff;
        }
    }
    return sumSquareDiff / nDiff;
}
