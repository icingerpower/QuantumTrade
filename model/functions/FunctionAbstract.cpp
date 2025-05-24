#include "FunctionAbstract.h"

QList<const FunctionAbstract *> FunctionAbstract::ALL_FUNCTIONS;

FunctionAbstract::Recorder::Recorder(const FunctionAbstract *function)
{
    ALL_FUNCTIONS << function;
    std::sort(ALL_FUNCTIONS.begin(), ALL_FUNCTIONS.end());
}

const QList<const FunctionAbstract *> &FunctionAbstract::allFunctions()
{
    return ALL_FUNCTIONS;
}
