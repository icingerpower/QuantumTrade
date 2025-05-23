#ifndef INDICATORVARIANCE_H
#define INDICATORVARIANCE_H

#include <QString>

#include "IndicatorAbstract.h"

class IndicatorVariance : public IndicatorAbstract
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
    QList<QMap<QString, QVariant>> possibleParams() const override;

};

#endif // INDICATORVARIANCE_H
