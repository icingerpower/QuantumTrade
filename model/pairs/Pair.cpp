#include "Pair.h"

Pair::Pair(const QString &symbol, const QString &currency)
{
    m_symbol = symbol;
    m_currency = currency;
}

QString Pair::name() const
{
    return m_symbol;
}

QString Pair::nameDataBase() const
{
    return m_symbol + m_currency;
}

const QString &Pair::symbol() const
{
    return m_symbol;
}

const QString &Pair::currency() const
{
    return m_currency;
}
