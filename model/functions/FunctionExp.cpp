#include <QtMath>

#include "FunctionExp.h"

RECORD_FUNCTION(FunctionExp);

QString FunctionExp::id() const
{
    return "functionExp";
}

QString FunctionExp::name() const
{
    return QObject::tr("Exp");
}

QString FunctionExp::description() const
{
    return QObject::tr("return the exponential log");
}

double FunctionExp::transform(double value, double)
{
    return qExp(value);
}
