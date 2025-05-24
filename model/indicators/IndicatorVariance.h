#ifndef INDICATORVARIANCE_H
#define INDICATORVARIANCE_H

#include <QString>

#include "IndicatorAbstract.h"

class IndicatorVariance : public IndicatorAbstract
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
    QList<QMap<QString, QVariant>> possibleParams() const override;

};

#endif // INDICATORVARIANCE_H
