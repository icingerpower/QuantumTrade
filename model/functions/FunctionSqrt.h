#ifndef FUNCTIONSQRT_H
#define FUNCTIONSQRT_H

#include "FunctionAbstract.h"

class FunctionSqrt : public FunctionAbstract
{
public:
    QString name() const override;
    QString description() const override;
    double transform(double value, double valuePrevious) override;
};

#endif // FUNCTIONSQRT_H
