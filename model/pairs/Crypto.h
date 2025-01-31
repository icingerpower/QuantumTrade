#ifndef CRYPTO_H
#define CRYPTO_H

#include "Pair.h"

class Crypto : public Pair
{
public:
    static Crypto * create(const QString &symbol, const QString &currency);
    Crypto(const QString &symbol, const QString &currency);
};

#endif // CRYPTO_H
