#include "FunctionLogReturn.h"

RECORD_FUNCTION(FunctionLogReturn);

QString FunctionLogReturn::name() const
{
    return QObject::tr("Log");
}

QString FunctionLogReturn::description() const
{
    return QObject::tr("return the exponential log");
}

double FunctionLogReturn::transform(double value, double valuePrevious)
{
    return qLn(value/valuePrevious);
}
