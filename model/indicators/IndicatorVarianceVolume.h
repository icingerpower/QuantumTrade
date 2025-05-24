#ifndef INDICATORVarianceVolume_H
#define INDICATORVarianceVolume_H

#include <QString>

#include "IndicatorVariance.h"

class IndicatorVarianceVolume : public IndicatorVariance
{
public:
    QString id() const override;
    QString name() const override;
    QString description() const override;
    double compute(
        std::deque<std::vector<double>> &queueOfValues,
        int colIndexLow,
        int colIndexHigh,
        int colIndexOpen,
        int colIndexClose,
        int colIndexVolume,
        const Tick *tick,
        const QMap<QString, QVariant> &params) const override;

};

#endif // INDICATORVarianceVolume_H
