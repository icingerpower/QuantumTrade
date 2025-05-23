#ifndef IndicatorVariancePositive_H
#define IndicatorVariancePositive_H

#include <QString>

#include "IndicatorVariance.h"

class IndicatorVariancePositive : public IndicatorVariance
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

#endif // IndicatorVariancePositive_H
