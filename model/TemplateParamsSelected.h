#ifndef TEMPLATEPARAMSSELECTED_H
#define TEMPLATEPARAMSSELECTED_H

#include <QSortFilterProxyModel>

class Tick;

class TemplateParamsSelected : public QSortFilterProxyModel
{
    Q_OBJECT

public:
    explicit TemplateParamsSelected(
        const QString &templateId, QObject *parent = nullptr);
    QVariant getValue(const QString &paramId) const;
    const Tick *getTick() const;
    Qt::ItemFlags flags(const QModelIndex& index) const override;
};


#endif // TEMPLATEPARAMSSELECTED_H
