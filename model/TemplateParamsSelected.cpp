#include "TemplateParams.h"

#include "TemplateParamsSelected.h"

TemplateParamsSelected::TemplateParamsSelected(
    const QString &templateId, QObject *parent)
{
    setSourceModel(TemplateParams::instance(templateId));
}

QVariant TemplateParamsSelected::getValue(const QString &paramId) const
{
    return static_cast<TemplateParams *>(sourceModel())->getValue(paramId);
}

const Tick *TemplateParamsSelected::getTick() const
{
    return static_cast<TemplateParams *>(sourceModel())->getTick();
}

Qt::ItemFlags TemplateParamsSelected::flags(const QModelIndex &index) const
{
    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}
