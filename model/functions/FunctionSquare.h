#ifndef FUNCTIONLOG_H
#define FUNCTIONLOG_H

#include "FunctionAbstract.h"

class FunctionSquare : public FunctionAbstract
{
public:
    QString name() const override;
    QString description() const override;
    double transform(double value, double valuePrevious) override;
};

#endif // FUNCTIONLOG_H
