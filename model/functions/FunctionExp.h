#ifndef FUNCTIONLOG_H
#define FUNCTIONLOG_H

#include "FunctionAbstract.h"

class FunctionExp : public FunctionAbstract
{
public:
    QString id() const override;
    QString name() const override;
    QString description() const override;
    double transform(double value, double valuePrevious) override;
};

#endif // FUNCTIONLOG_H
