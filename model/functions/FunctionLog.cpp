#include <QtMath>

#include "FunctionLog.h"

RECORD_FUNCTION(FunctionLog);

QString FunctionLog::name() const
{
    return QObject::tr("Log");
}

QString FunctionLog::description() const
{
    return QObject::tr("return the exponential log");
}

double FunctionLog::transform(double value, double)
{
    return qLn(value);
}
