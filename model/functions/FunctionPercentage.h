#ifndef FUNCTIONPERCENTAGE_H
#define FUNCTIONPERCENTAGE_H

#include "FunctionAbstract.h"

class FunctionPercentage : public FunctionAbstract
{
public:
    QString name() const override;
    QString description() const override;
    double transform(double value, double valuePrevious) override;
};

#endif // FUNCTIONPERCENTAGE_H
