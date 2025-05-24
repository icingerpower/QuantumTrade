#include "FunctionPercentage.h"

RECORD_FUNCTION(FunctionPercentage);

QString FunctionPercentage::id() const
{
    return "FunctionPercentage";
}

QString FunctionPercentage::name() const
{
    return QObject::tr("Percentage");
}

QString FunctionPercentage::description() const
{
    return QObject::tr("The value in %");
}

double FunctionPercentage::transform(double value, double valuePrevious)
{
    if (valuePrevious == 0.0)
    {
        return std::numeric_limits<double>::quiet_NaN();
    }
    return (value - valuePrevious) / valuePrevious;
}
