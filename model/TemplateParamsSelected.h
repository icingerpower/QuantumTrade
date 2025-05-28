#ifndef TEMPLATEPARAMSSELECTED_H
#define TEMPLATEPARAMSSELECTED_H

#include <QSortFilterProxyModel>

class TemplateParamsSelected : public QSortFilterProxyModel
{
    Q_OBJECT

public:
    explicit TemplateParamsSelected(
        const QString &templateId, QObject *parent = nullptr);
    bool filterAcceptsRow(int sourceRow,
                          const QModelIndex &sourceParent) const override;
    bool filterAcceptsColumn(int source_column,
                             const QModelIndex &source_parent) const override;
    Qt::ItemFlags flags(const QModelIndex& index) const override;
};


#endif // TEMPLATEPARAMSSELECTED_H
