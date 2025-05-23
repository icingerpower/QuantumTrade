// IndicatorMACD.h
#ifndef INDICATORMACD_H
#define INDICATORMACD_H

#include <QString>
#include <QPair>
#include <QVariant>
#include "IndicatorAbstract.h"

class IndicatorMACD : public IndicatorAbstract
{
public:
    // Parameter IDs
    static QString PAR_ID_FAST_PERIOD;
    static QPair<QString, QString> PAR_ID_NAME_FAST_PERIOD;
    static QString PAR_ID_SLOW_PERIOD;
    static QPair<QString, QString> PAR_ID_NAME_SLOW_PERIOD;
    static QString PAR_ID_SIGNAL_PERIOD;
    static QPair<QString, QString> PAR_ID_NAME_SIGNAL_PERIOD;

    QString name() const override;
    QString description() const override;
    double compute(
        std::deque<std::vector<double>>& queueOfValues,
        int colIndexValue,
        int colIndexVolume,
        int colIndexOpen,
        int colIndexClose,
        const Tick* tick,
        const QMap<QString, QVariant>& params) const override;
    QList<QMap<QString, QVariant>> possibleParams() const override;
};

#endif // INDICATORMACD_H
