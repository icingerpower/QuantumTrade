#ifndef STOCK_H
#define STOCK_H

#include "Pair.h"

class Stock : public Pair
{
public:
    static Stock * create(const QString &symbol, const QString &currency);
    Stock(const QString &symbol, const QString &currency);
};

#endif // STOCK_H
