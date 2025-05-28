#include "model/TemplateManager.h"
#include "model/TradingPairsSelected.h"
#include "model/TemplateParamsSelected.h"
#include "model/IndicatorsSelected.h"

#include "PaneData.h"
#include "ui_PaneData.h"

PaneData::PaneData(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PaneData)
{
    ui->setupUi(this);
    ui->listViewTemplates->setModel(TemplateManager::instance());
    _connectSlots();
    ui->listViewTemplates->setCurrentIndex(
        TemplateManager::instance()->index(0,0));
}

PaneData::~PaneData()
{
    delete ui;
}

void PaneData::_connectSlots()
{
    connect(ui->listViewTemplates->selectionModel(),
            &QItemSelectionModel::selectionChanged,
            this,
            &PaneData::onTemplateSelected);
}

void PaneData::onTemplateSelected(
    const QItemSelection &selected, const QItemSelection &deselected)
{
    if (selected.size() > 0 || deselected.size() > 0)
    {
        auto model = ui->tableViewPairs->model();
        if (model != nullptr)
        {
            ui->tableViewPairs->setModel(nullptr);
            model->deleteLater();
        }
        if (selected.size() > 0)
        {
            auto firstIndex = selected.indexes().first();
            const auto &templateId = TemplateManager::instance()->getId(firstIndex);

            auto templateParamsModel = new TemplateParamsSelected{templateId, ui->tableViewPairs};
            ui->tableViewParams->setModel(templateParamsModel);

            auto tradingPairsModel = new TradingPairsSelected{templateId, ui->tableViewPairs};
            ui->tableViewPairs->setModel(tradingPairsModel);

            auto indicatorsModel = new IndicatorsSelected{templateId, ui->listViewIndicators};
            ui->listViewIndicators->setModel(indicatorsModel);
        }
    }

}
