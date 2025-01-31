#ifndef PAIR_H
#define PAIR_H

#include "VariableAbstract.h"

class Pair : public VariableAbstract
{
public:
    Pair(const QString &symbol, const QString &currency);

    QString name() const override;
    QString nameDataBase() const override;

    const QString &symbol() const;

    const QString &currency() const;

private:
    QString m_symbol;
    QString m_currency;
};

#endif // PAIR_H
