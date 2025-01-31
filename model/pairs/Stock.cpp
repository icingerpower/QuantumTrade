#include "Stock.h"

Stock *Stock::create(const QString &symbol, const QString &currency)
{
    static QHash<QString, QHash<QString, QSharedPointer<Stock>>> cryptos;
    cryptos[symbol];
    if (!cryptos[symbol].contains(currency))
    {
        cryptos[symbol][currency] = QSharedPointer<Stock>{new Stock{symbol, currency}};
    }
    return cryptos[symbol][currency].data();
}

Stock::Stock(const QString &symbol, const QString &currency)
    : Pair(symbol, currency)
{

}
