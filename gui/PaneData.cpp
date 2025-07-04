#include <QTimer>
#include <QChart>
#include <QtCharts/QChartView>
#include <QtCharts/QValueAxis>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QCandlestickSeries>
#include <QtCharts/QCandlestickSet>

#include "../common/workingdirectory/WorkingDirectoryManager.h"

#include "model/TemplateManager.h"
#include "model/TradingPairsSelected.h"
#include "model/TemplateParamsSelected.h"
#include "model/TemplateParams.h"
#include "model/IndicatorsSelected.h"
#include "model/ApiParams.h"
#include "model/readers/StreamReaderAbstract.h"
#include "model/pairs/VariableAbstract.h"

#include "PaneData.h"
#include "ui_PaneData.h"

PaneData::PaneData(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PaneData)
{
    ui->setupUi(this);
    ui->listViewTemplates->setModel(TemplateManager::instance());
    ui->chartViewPrice->setChart(new QChart{});
    ui->chartViewPrice->chart()->setBackgroundBrush(
        Qt::lightGray);
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
            disconnect(m_connectionPair);
            m_connectionPair = connect(
                ui->tableViewPairs->selectionModel(),
                &QItemSelectionModel::selectionChanged,
                this,
                &PaneData::onPairSelected);

            auto indicatorsModel = new IndicatorsSelected{templateId, ui->listViewIndicators};
            ui->listViewIndicators->setModel(indicatorsModel);
        }
    }
}

void PaneData::onPairSelected(const QItemSelection &selected, const QItemSelection &deselected)
{
    if (selected.size() > 0)
    {
        QString symbol{selected.indexes().first().data().toString()};
        auto variableAvailability = StreamReaderAbstract::allAvailableVariables()[symbol];
        const auto *tick = getTemplateParamsSelected()->getTick();
        const auto &dateTimeStart = variableAvailability.variable->readDateTimeStart(*tick);
        const auto &dateTimeEnd = variableAvailability.variable->readDateTimeEnd(*tick);
        ui->dateTimeEditFrom->setDateTime(dateTimeStart);
        ui->dateTimeEditTo->setDateTime(dateTimeEnd);
        QStringList valueTypes{
                               VariableAbstract::TYPE_VALUE_OPEN.id
                               , VariableAbstract::TYPE_VALUE_MAX.id
                               , VariableAbstract::TYPE_VALUE_MIN.id
                               , VariableAbstract::TYPE_VALUE_CLOSE.id
                               , VariableAbstract::TYPE_VALUE_VOLUME.id};
        auto data = variableAvailability.variable->readData(
            *tick,
            valueTypes,
            dateTimeStart,
            dateTimeEnd);
        QChart *chart = new QChart();
        chart->setBackgroundBrush(Qt::lightGray);

        const QMap<QDateTime,double>& openMap  = (*data)[VariableAbstract::TYPE_VALUE_OPEN.id];
        const QMap<QDateTime,double>& highMap  = (*data)[VariableAbstract::TYPE_VALUE_MAX.id];
        const QMap<QDateTime,double>& lowMap   = (*data)[VariableAbstract::TYPE_VALUE_MIN.id];
        const QMap<QDateTime,double>& closeMap = (*data)[VariableAbstract::TYPE_VALUE_CLOSE.id];
        const QMap<QDateTime,double>& volMap   = (*data)[VariableAbstract::TYPE_VALUE_VOLUME.id];

        QList<QDateTime> times = openMap.keys();
        std::sort(times.begin(), times.end());
        if (times.isEmpty()) return;

        // 2) QCandlestickSeries
        QCandlestickSeries* candleSeries = new QCandlestickSeries();
        candleSeries->setName("Price");
        candleSeries->setIncreasingColor(QColor(0,200,83));
        candleSeries->setDecreasingColor(QColor(244,67,54));
        candleSeries->setBodyOutlineVisible(false);

        for (const QDateTime& dt : times) {
            qreal o = openMap[dt];
            qreal h = highMap[dt];
            qreal l = lowMap[dt];
            qreal c = closeMap[dt];
            candleSeries->append(new QCandlestickSet(o,h,l,c));
        }

        // 3) QBarSeries pour le volume
        QBarSet* volSet = new QBarSet("Vol");
        for (const QDateTime& dt : times) {
            *volSet << volMap[dt];
        }
        QBarSeries* barSeries = new QBarSeries();
        barSeries->append(volSet);

        // 4) Création du chart
        chart->setTitle(symbol + " — Candlestick + Volume");
        chart->addSeries(candleSeries);
        chart->addSeries(barSeries);

        // 5) Axe X (catégories)
        QStringList categories;
        categories.reserve(times.size());
        for (const QDateTime& dt : times)
            categories << dt.toString("MM-dd hh:mm");
        QBarCategoryAxis* axisX = new QBarCategoryAxis();
        axisX->append(categories);
        axisX->setLabelsAngle(-90);
        axisX->setGridLineVisible(false);

        chart->addAxis(axisX, Qt::AlignBottom);
        candleSeries->attachAxis(axisX);
        barSeries->attachAxis(axisX);

        // 6) Axe Y gauche (prix)
        QValueAxis* axisYPrice = new QValueAxis();
        axisYPrice->setTitleText("Price");
        axisYPrice->setLabelFormat("%.2f");

        chart->addAxis(axisYPrice, Qt::AlignLeft);
        candleSeries->attachAxis(axisYPrice);

        // 7) Axe Y droit (volume)
        QValueAxis* axisYVol = new QValueAxis();
        axisYVol->setTitleText("Volume");
        axisYVol->setGridLineVisible(false);

        chart->addAxis(axisYVol, Qt::AlignRight);
        barSeries->attachAxis(axisYVol);

        // 8) Affectation au QChartView
        ui->chartViewPrice->setChart(chart);
        ui->chartViewPrice->setRenderHint(QPainter::Antialiasing);
    }
    else if (deselected.size() > 0)
    {


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
