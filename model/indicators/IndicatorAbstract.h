#ifndef FUNCTIONABSTRACT_H
#define FUNCTIONABSTRACT_H

#include <QList>
#include <deque>
#include <vector>
#include <QMap>
#include <QVariant>

class Tick;

class IndicatorAbstract
{
public:
    static QList<const IndicatorAbstract *> &allIndicators();
    static QString PAR_ID_SIZE_SAMPLE;
    static QPair<QString, QString> PAR_ID_NAME_SIZE_SAMPLE;
    virtual QString id() const = 0;
    virtual QString name() const = 0;
    virtual QString description() const = 0;
    virtual double compute(
        std::deque<std::vector<double>> &queueOfValues,
        int colIndexLow,
        int colIndexHigh,
        int colIndexOpen,
        int colIndexClose,
        int colIndexVolume,
        const Tick *tick,
        const QMap<QString, QVariant> &params) const = 0;
    virtual QList<QMap<QString, QVariant>> possibleParams() const = 0; // Allow the indicator to be run / tester with a different set of params
    class Recorder{
    public:
        Recorder(const IndicatorAbstract *indicator);
    };

private:
    static QList<const IndicatorAbstract *> ALL_INDICATORS;
};

#define RECORD_INDICATOR(INDICATOR_CLASS) \
INDICATOR_CLASS instance##INDICATOR_CLASS; \
    INDICATOR_CLASS::Recorder recorder##INDICATOR_CLASS{&instance##INDICATOR_CLASS};


#endif // FUNCTIONABSTRACT_H
