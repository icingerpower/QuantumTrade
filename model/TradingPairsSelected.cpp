#include "TradingPairs.h"

#include "TradingPairsSelected.h"

TradingPairsSelected::TradingPairsSelected(
    const QString &templateId, QObject *parent)
{
    setSourceModel(TradingPairs::instance(templateId));
}

QMultiHash<QString, VariableAvailability> TradingPairsSelected::getSelectedVariables() const
{
    TradingPairs *tradingPairs = static_cast<TradingPairs *>(sourceModel());
    return tradingPairs->getSelectedVariables();
}

bool TradingPairsSelected::filterAcceptsRow(
    int sourceRow, const QModelIndex &) const
{
    TradingPairs *tradingPairs = static_cast<TradingPairs *>(sourceModel());
    return tradingPairs->isRowChecked(sourceRow);
}

bool TradingPairsSelected::filterAcceptsColumn(
    int source_column, const QModelIndex &) const
{
    return source_column == 0;
}

Qt::ItemFlags TradingPairsSelected::flags(const QModelIndex &index) const
{
    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}
