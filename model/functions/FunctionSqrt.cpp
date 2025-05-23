#include <QtMath>

#include "FunctionSqrt.h"

RECORD_FUNCTION(FunctionSqrt);

QString FunctionSqrt::name() const
{
    return QObject::tr("Sqrt");
}

QString FunctionSqrt::description() const
{
    return QObject::tr("Return the square root");
}

double FunctionSqrt::transform(double value, double)
{
    return qSqrt(value);
}
