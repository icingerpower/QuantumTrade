#ifndef VARIABLEAVAILABLILITY_H
#define VARIABLEAVAILABLILITY_H

#include <QString>
#include <QSet>
class StreamReaderAbstract;

class VariableAbstract;
struct VariableAvailability{
    VariableAbstract *variable;
    QSet<QString> tickIds;
    const StreamReaderAbstract *streamReader;
    //QSet<QString> tickIdsHistory; //We assume all have history
    //QSet<QString> tickIdsNoHistory;
};

#endif // VARIABLEAVAILABLILITY_H
