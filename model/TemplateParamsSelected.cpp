#include "TemplateParams.h"

#include "TemplateParamsSelected.h"

TemplateParamsSelected::TemplateParamsSelected(
    const QString &templateId, QObject *parent)
{
    setSourceModel(new TemplateParams{templateId, this});
}

bool TemplateParamsSelected::filterAcceptsRow(
    int sourceRow, const QModelIndex &sourceParent) const
{
    return true;
}

bool TemplateParamsSelected::filterAcceptsColumn(
    int source_column, const QModelIndex &source_parent) const
{
    return true;
}

Qt::ItemFlags TemplateParamsSelected::flags(const QModelIndex &index) const
{
    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}
