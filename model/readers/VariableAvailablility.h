#ifndef VARIABLEAVAILABLILITY_H
#define VARIABLEAVAILABLILITY_H

#include <QString>
#include <QSet>

class VariableAbstract;
struct VariableAvailability{
    VariableAbstract *variable;
    QSet<QString> tickIds;
    //QSet<QString> tickIdsHistory; //We assume all have history
    //QSet<QString> tickIdsNoHistory;
};

#endif // VARIABLEAVAILABLILITY_H
