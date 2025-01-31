#include "TradingPairs.h"

TradingPairs::TradingPairs(QObject *parent)
    : QAbstractTableModel(parent)
{}

QVariant TradingPairs::headerData(int section, Qt::Orientation orientation, int role) const
{
    // FIXME: Implement me!
}

int TradingPairs::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    // FIXME: Implement me!
}

int TradingPairs::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    // FIXME: Implement me!
}

QVariant TradingPairs::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    // FIXME: Implement me!
    return QVariant();
}
