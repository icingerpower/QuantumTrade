#ifndef TRADINGPAIRSSELECTED_H
#define TRADINGPAIRSSELECTED_H

#include <QSortFilterProxyModel>

class TradingPairsSelected : public QSortFilterProxyModel
{
    Q_OBJECT

public:
    explicit TradingPairsSelected(
        const QString &templateId, QObject *parent = nullptr);
    bool filterAcceptsRow(int sourceRow,
                          const QModelIndex &sourceParent) const override;
    bool filterAcceptsColumn(int source_column,
                             const QModelIndex &source_parent) const override;
};

#endif // TRADINGPAIRSSELECTED_H
