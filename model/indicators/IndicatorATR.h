#ifndef INDICATORATR_H
#define INDICATORATR_H

#include <QString>
#include <QVariant>
#include "IndicatorAbstract.h"

class IndicatorATR : public IndicatorAbstract
{
public:
    QString name() const override;
    QString description() const override;
    double compute(
        std::deque<std::vector<double>>& queueOfValues,
        int /*colIndexValue*/,
        int /*colIndexVolume*/,
        int /*colIndexOpen*/,
        int colIndexClose,
        const Tick*,
        const QMap<QString, QVariant>& params) const override;
    QList<QMap<QString, QVariant>> possibleParams() const override;
};

#endif // INDICATORATR_H
