#ifndef FUNCTIONABSTRACT_H
#define FUNCTIONABSTRACT_H

#include <QList>
#include <deque>
#include <vector>
#include <QMap>
#include <QVariant>

class Tick;

class FunctionAbstract
{
public:
    virtual QString name() const = 0;
    virtual QString description() const = 0;
    virtual double transform(double value, double valuePrevious) = 0;
    class Recorder{
    public:
        Recorder(const FunctionAbstract *streamReader);
    };

private:
    static QList<const FunctionAbstract *> ALL_FUNCTIONS;
};

#define RECORD_FUNCTION(FUNCTION_CLASS) \
FUNCTION_CLASS instance##FUNCTION_CLASS; \
    FUNCTION_CLASS::Recorder recorder##FUNCTION_CLASS{&instance##FUNCTION_CLASS};

struct AcpResults
{
};

class FunctionACP
{
public:
    virtual void init(std::vector<double> rowValues,
                      int rowCount);
    virtual void update(std::vector<double> rowValues,
                        int indCurrent);
    virtual AcpResults computeFinal();
};

#endif // FUNCTIONABSTRACT_H
