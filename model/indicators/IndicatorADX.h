#ifndef INDICATORADX_H
#define INDICATORADX_H

#include <QString>
#include <QVariant>
#include "IndicatorAbstract.h"

class IndicatorADX : public IndicatorAbstract {
public:
    QString name() const override;
    QString description() const override;
    double compute(
        std::deque<std::vector<double>>& queueOfValues,
        int colIndexHigh,
        int colIndexLow,
        int colIndexClose,
        int colIndexVolume,
        const Tick*,
        const QMap<QString, QVariant>& params) const override;
    QList<QMap<QString, QVariant>> possibleParams() const override;
};

#endif // INDICATORADX_H
