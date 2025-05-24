#ifndef INDICATORBBW_H
#define INDICATORBBW_H

#include <QString>
#include <QPair>
#include <QVariant>
#include "IndicatorAbstract.h"

class IndicatorBBW : public IndicatorAbstract
{
public:
    // Parameter for the number of standard deviations
    static QString PAR_ID_STDDEV_MULTIPLIER;
    static QPair<QString, QString> PAR_ID_NAME_STDDEV_MULTIPLIER;

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
        const Tick* tick,
        const QMap<QString, QVariant>& params) const override;
    QList<QMap<QString, QVariant>> possibleParams() const override;
};

#endif // INDICATORBBW_H
