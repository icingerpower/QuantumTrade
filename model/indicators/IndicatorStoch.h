// IndicatorStoch.h
#ifndef INDICATORSTOCH_H
#define INDICATORSTOCH_H

#include <QString>
#include <QPair>
#include <QVariant>
#include "IndicatorAbstract.h"

class IndicatorStoch : public IndicatorAbstract
{
public:
    // Parameter IDs
    static QString PAR_ID_K_PERIOD;
    static QPair<QString, QString> PAR_ID_NAME_K_PERIOD;
    static QString PAR_ID_D_PERIOD;
    static QPair<QString, QString> PAR_ID_NAME_D_PERIOD;

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

#endif // INDICATORSTOCH_H
