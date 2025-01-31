#include "Currency.h"

Currency *Currency::create(
    const QString &symbol, const QString &currency)
{
    static QHash<QString, QHash<QString, QSharedPointer<Currency>>> cryptos;
    cryptos[symbol];
    if (!cryptos[symbol].contains(currency))
    {
        cryptos[symbol][currency] = QSharedPointer<Currency>{new Currency{symbol, currency}};
    }
    return cryptos[symbol][currency].data();
}

Currency::Currency(const QString &symbol, const QString &currency)
    : Pair(symbol, currency)
{

}
