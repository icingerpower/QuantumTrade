#ifndef IndicatorVarianceNegative_H
#define IndicatorVarianceNegative_H

#include <QString>

#include "IndicatorVariance.h"

class IndicatorVarianceNegative : public IndicatorVariance
{
public:
    QString name() const override;
    QString description() const override;
    double compute(
        std::deque<std::vector<double>> &queueOfValues,
        int colIndexValue,
        int colIndexVolume,
        int colIndexOpen,
        int colIndexClose,
        const Tick *tick,
        const QMap<QString, QVariant> &params) const override;

};

#endif // IndicatorVarianceNegative_H
