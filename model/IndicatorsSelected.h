#ifndef INDICATORSSELECTED_H
#define INDICATORSSELECTED_H

#include <QSortFilterProxyModel>

class IndicatorsSelected : public QSortFilterProxyModel
{
    Q_OBJECT

public:
    explicit IndicatorsSelected(
        const QString &templateId, QObject *parent = nullptr);
    bool filterAcceptsRow(int sourceRow,
                          const QModelIndex &sourceParent) const override;
    Qt::ItemFlags flags(const QModelIndex& index) const override;
};

#endif // INDICATORSSELECTED_H
