#ifndef CURRENCY_H
#define CURRENCY_H

#include "Pair.h"

class Currency : public Pair
{
public:
    static Currency * create(const QString &symbol, const QString &currency);
    Currency(const QString &symbol, const QString &currency);
};


#endif // CURRENCY_H
