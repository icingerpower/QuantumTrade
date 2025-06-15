#include "Indicators.h"

#include "IndicatorsSelected.h"

IndicatorsSelected::IndicatorsSelected(
    const QString &templateId, QObject *parent)
{
    setSourceModel(Indicators::instance(templateId));
}

bool IndicatorsSelected::filterAcceptsRow(
    int, const QModelIndex &) const
{
    return true;
}

Qt::ItemFlags IndicatorsSelected::flags(const QModelIndex &index) const
{
    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}
