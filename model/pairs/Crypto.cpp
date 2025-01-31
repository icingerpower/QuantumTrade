#include <QSharedPointer>

#include "Crypto.h"

Crypto *Crypto::create(
    const QString &symbol, const QString &currency)
{
    static QHash<QString, QHash<QString, QSharedPointer<Crypto>>> cryptos;
    cryptos[symbol];
    if (!cryptos[symbol].contains(currency))
    {
        cryptos[symbol][currency] = QSharedPointer<Crypto>{new Crypto{symbol, currency}};
    }
    return cryptos[symbol][currency].data();
}

Crypto::Crypto(const QString &symbol, const QString &currency)
    : Pair(symbol, currency)
{

}
