#ifndef INDICATORATR_H
#define INDICATORATR_H

#include <QString>
#include <QVariant>
#include "IndicatorAbstract.h"

class IndicatorATR : public IndicatorAbstract
{
public:
    QString id() const override;
    QString name() const override;
    QString description() const override;
    double compute(
        std::deque<std::vector<double>>& queueOfValues,
        int colIndexLow,
        int colIndexHigh,
        int colIndexOpen,
        int colIndexClose,
        int colIndexVolume,
        const Tick*,
        const QMap<QString, QVariant>& params) const override;
    QList<QMap<QString, QVariant>> possibleParams() const override;
};

#endif // INDICATORATR_H
