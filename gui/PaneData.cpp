#include <QTimer>

#include "model/TemplateManager.h"
#include "model/TradingPairsSelected.h"
#include "model/TemplateParamsSelected.h"
#include "model/TemplateParams.h"
#include "model/IndicatorsSelected.h"
#include "model/ApiParams.h"
#include "model/readers/StreamReaderAbstract.h"

#include "PaneData.h"
#include "ui_PaneData.h"

PaneData::PaneData(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PaneData)
{
    ui->setupUi(this);
    ui->listViewTemplates->setModel(TemplateManager::instance());
    ui->buttonRun->setEnabled(false);
    _connectSlots();
    ui->listViewTemplates->setCurrentIndex(
        TemplateManager::instance()->index(0,0));
    m_job = QSharedPointer<Job>{new Job};
}

PaneData::~PaneData()
{
    delete ui;
}

TradingPairsSelected *PaneData::getTradingPairsSelected() const
{
    return static_cast<TradingPairsSelected *>(
        ui->tableViewPairs->model());
}

TemplateParamsSelected *PaneData::getTemplateParamsSelected() const
{
    return static_cast<TemplateParamsSelected *>(
        ui->tableViewParams->model());
}

void PaneData::_connectSlots()
{
    connect(ui->buttonRun,
            &QPushButton::clicked,
            this,
            &PaneData::run);
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
        auto modelPairs = ui->tableViewPairs->model();
        if (modelPairs != nullptr)
        {
            ui->tableViewPairs->setModel(nullptr);
            modelPairs->deleteLater();
        }
        auto modelParams = ui->tableViewParams->model();
        if (modelParams != nullptr)
        {
            ui->tableViewParams->setModel(nullptr);
            modelParams->deleteLater();
        }
        auto modelIndicators = ui->listViewIndicators->model();
        if (modelIndicators != nullptr)
        {
            ui->listViewIndicators->setModel(nullptr);
            modelIndicators->deleteLater();
        }
        if (selected.size() > 0)
        {
            ui->buttonRun->setEnabled(true);
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

void PaneData::run(bool start)
{
    if (start)
    {
        m_job->start();
        ui->listViewTemplates->setEnabled(false);
        auto selVariables = getTradingPairsSelected()->getSelectedVariables();
        auto streamerVariables = getTemplateParamsSelected();
        const auto &symbols = selVariables.uniqueKeys();
        for (const auto &symbol : symbols)
        {
            if (symbol == "NVDA")
            {
            const auto &allVariableAvailable = selVariables.values(symbol);
            const auto &firstVariableAvailable = allVariableAvailable[0];
            const auto &params = ApiParams::instance()->getParams(
                firstVariableAvailable.streamReader);
            const auto *tick = getTemplateParamsSelected()->getTick();
            int nDays = getTemplateParamsSelected()->getValue(TemplateParams::PARAM_DAYS_DATA).toInt();
            const QDate &dateTo = QDate::currentDate();
            const QDate &dateFrom = dateTo.addDays(-nDays);
            firstVariableAvailable.streamReader->readData(
                params,
                *tick,
                firstVariableAvailable.variable,
                dateFrom,
                dateTo,
                m_job);
            }
        }
    }
    else
    {
        m_job->stop();
        ui->listViewTemplates->setEnabled(true);
        ui->buttonRun->setEnabled(false);
        QTimer::singleShot(2000, [this](){
            ui->buttonRun->setEnabled(true);
        });
    }
}
