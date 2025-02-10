#ifndef VARIABLEAVAILABLILITY_H
#define VARIABLEAVAILABLILITY_H

#include <QString>
#include <QSet>

class VariableAbstract;
struct VariableAvailability{
    VariableAbstract *variable;
    QSet<QString> tickIdsHistory;
    QSet<QString> tickIdsNoHistory;
};

#endif // VARIABLEAVAILABLILITY_H
