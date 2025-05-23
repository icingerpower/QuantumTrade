#include "FunctionAbstract.h"

QList<const FunctionAbstract *> FunctionAbstract::ALL_FUNCTIONS;

FunctionAbstract::Recorder::Recorder(const FunctionAbstract *function)
{
    ALL_FUNCTIONS << function;
}
