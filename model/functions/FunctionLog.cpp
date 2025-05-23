#include <QtMath>

#include "FunctionLog.h"

RECORD_FUNCTION(FunctionLog);

QString FunctionLog::name() const
{
    return "Log";
}

QString FunctionLog::description() const
{
    return "return the exponential log";
}

double FunctionLog::transform(double value)
{
    return qLn(value);
}
