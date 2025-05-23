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

    QString name() const override;
    QString description() const override;
    double compute(
        std::deque<std::vector<double>>& queueOfValues,
        int colIndexValue,
        int /*colIndexVolume*/,
        int /*colIndexOpen*/,
        int /*colIndexClose*/,
        const Tick*,
        const QMap<QString, QVariant>& params) const override;
    QList<QMap<QString, QVariant>> possibleParams() const override;
};

#endif // INDICATORSTOCH_H
