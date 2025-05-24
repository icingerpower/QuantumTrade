#ifndef FUNCTIONLOGRETURN_H
#define FUNCTIONLOGRETURN_H

#include "FunctionAbstract.h"

class FunctionLogReturn : public FunctionAbstract
{
public:
    QString id() const override;
    QString name() const override;
    QString description() const override;
    double transform(double value, double valuePrevious) override;
};

#endif // FUNCTIONLOGRETURN_H
