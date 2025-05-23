#include <QtMath>

#include "FunctionSquare.h"

RECORD_FUNCTION(FunctionSquare);

QString FunctionSquare::name() const
{
    return "Square";
}

QString FunctionSquare::description() const
{
    return "return the exponential log";
}

double FunctionSquare::transform(double value)
{
    return qLn(value);
}
