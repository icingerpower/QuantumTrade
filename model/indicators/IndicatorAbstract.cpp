#include <QObject>

#include "IndicatorAbstract.h"

QString IndicatorAbstract::PAR_ID_SIZE_SAMPLE{"sizeSample"};
QPair<QString, QString> IndicatorAbstract::PAR_ID_NAME_SIZE_SAMPLE{PAR_ID_SIZE_SAMPLE, QObject::tr("Sample size")};

QList<const IndicatorAbstract *> IndicatorAbstract::ALL_INDICATORS;

IndicatorAbstract::Recorder::Recorder(const IndicatorAbstract *indicator)
{
    IndicatorAbstract::ALL_INDICATORS << indicator;
}
