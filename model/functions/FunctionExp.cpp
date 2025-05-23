#include <QtMath>

#include "FunctionExp.h"

RECORD_FUNCTION(FunctionExp);

QString FunctionExp::name() const
{
    return "Exp";
}

QString FunctionExp::description() const
{
    return "return the exponential log";
}

double FunctionExp::transform(double value)
{
    return qExp(value);
}
