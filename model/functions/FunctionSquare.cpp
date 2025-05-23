#include <QtMath>

#include "FunctionSquare.h"

RECORD_FUNCTION(FunctionSquare);

QString FunctionSquare::name() const
{
    return QObject::tr("Square");
}

QString FunctionSquare::description() const
{
    return QObject::tr("return the exponential log");
}

double FunctionSquare::transform(double value, double)
{
    return qLn(value);
}
