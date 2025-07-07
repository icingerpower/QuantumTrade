#include <QTimer>
#include <QtCharts/QChart>
#include <QtCharts/QValueAxis>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QCandlestickSeries>
#include <QtCharts/QCandlestickSet>
#include <QtCharts/QDateTimeAxis>
#include <QtCharts/QLogValueAxis>
#include <QtCharts/QLineSeries>

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
    for (auto chartView : getChartViews())
    {
        chartView->setChart(new QChart{});
        chartView->chart()->setBackgroundBrush(
                    Qt::lightGray);
    }
    ui->buttonRun->setEnabled(false);
    _connectSlots();
    ui->listChartType->setCurrentRow(0);
    ui->listViewTemplates->setCurrentIndex(
        TemplateManager::instance()->index(0,0));
    m_job = QSharedPointer<Job>{new Job};
}

QList<QChartView *> PaneData::getChartViews()
{
    return QList<QChartView *>{
        ui->chartViewPrice
                , ui->chartViewPriceLog
                , ui->chartViewVolume
    };
}

PaneData::~PaneData()
{
    m_job->stop();
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

void PaneData::writeMessage(const QString &message)
{
    _writeMessage(message, Qt::white);
}

void PaneData::_writeMessage(const QString &message, const QColor &color)
{
    QTextCursor cursor(ui->textEditLog->document());
    cursor.movePosition(QTextCursor::End);

    // 2) Prepare a char format with the desired color
    QTextCharFormat fmt;
    fmt.setForeground(color);

    // 3) Insert the text (and a newline) with that format
    cursor.insertText(message + QLatin1Char('\n'), fmt);

    // 4) Make sure the QTextEdit scrolls to show the newly appended text
    ui->textEditLog->setTextCursor(cursor);
    ui->textEditLog->ensureCursorVisible();
}

void PaneData::writeMessageError(const QString &message)
{
    _writeMessage(message, Qt::red);
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
    QList<QChart *> currentCharts;
    for (auto chartView : getChartViews())
    {
        currentCharts << chartView->chart();
    }
    if (selected.size() > 0)
    {
        QString symbol{selected.indexes().first().data().toString()};
        auto variableAvailability = StreamReaderAbstract::allAvailableVariables()[symbol];
        const auto *tick = getTemplateParamsSelected()->getTick();
        const auto &dateTimeStart = variableAvailability.variable->readDateTimeStart(*tick);
        const auto &dateTimeEnd = variableAvailability.variable->readDateTimeEnd(*tick);
        QChart *chartPrice = new QChart();
        QChart *chartVol = new QChart();
        QChart *chartPriceLog = new QChart();
        chartPrice->setBackgroundBrush(Qt::lightGray);
        chartVol->setBackgroundBrush(Qt::lightGray);
        chartPriceLog->setBackgroundBrush(Qt::lightGray);
        if (dateTimeStart.isValid())
        {
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

            const QMap<QDateTime,double>& openMap  = (*data)[VariableAbstract::TYPE_VALUE_OPEN.id];
            const QMap<QDateTime,double>& highMap  = (*data)[VariableAbstract::TYPE_VALUE_MAX.id];
            const QMap<QDateTime,double>& lowMap   = (*data)[VariableAbstract::TYPE_VALUE_MIN.id];
            const QMap<QDateTime,double>& closeMap = (*data)[VariableAbstract::TYPE_VALUE_CLOSE.id];
            const QMap<QDateTime,double>& volMap   = (*data)[VariableAbstract::TYPE_VALUE_VOLUME.id];
            //auto minMaxOpen = std::minmax_element(openMap.begin(), openMap.end());
            //auto minMaxHigh = std::minmax_element(highMap.begin(), highMap.end());
            //auto minMaxLow = std::minmax_element(lowMap.begin(), lowMap.end());
            //auto minMaxClose = std::minmax_element(closeMap.begin(), closeMap.end());
            if (openMap.size() > 0)
            {

                QList<QDateTime> times = openMap.keys();
                std::sort(times.begin(), times.end());

                auto *candleSeries    = new QCandlestickSeries();
                auto *candleSeriesLog = new QCandlestickSeries();
                for (auto *cs : {candleSeries, candleSeriesLog}) {
                    cs->setName("Price");
                    cs->setIncreasingColor(QColor(0,200,83));
                    cs->setDecreasingColor(QColor(244,67,54));
                    cs->setBodyOutlineVisible(false);
                }
                //auto minVal = lowMap.first();
                //auto maxVal = highMap.first();

                for (const auto &dt : times) {
                    qreal o = openMap[dt],
                          h = highMap[dt],
                          l = lowMap[dt],
                          c = closeMap[dt];
                    //minVal = qMin(minVal, lowMap[dt]);
                    //maxVal = qMax(maxVal, highMap[dt]);
                    qint64 ts = dt.toMSecsSinceEpoch();  // <-- crucial!
                    candleSeries   ->append(new QCandlestickSet(o, h, l, c, ts));
                    candleSeriesLog->append(new QCandlestickSet(o, h, l, c, ts));
                }

                // 2) Create **two separate** QDateTimeAxis, each ranged to [start, end]:
                auto *axisXprice     = new QDateTimeAxis();
                auto *axisXvol     = new QDateTimeAxis();
                auto *axisXlog  = new QDateTimeAxis();
                for (auto *ax : {axisXvol, axisXprice, axisXlog}) {
                    ax->setFormat("yy-MM-dd");
                    ax->setLabelsAngle(-45);
                    //ax->setRange(dateTimeStart, dateTimeEnd);
                    ax->setMin(dateTimeStart);
                }

                // 3) Linear price chart
                chartPrice->setBackgroundBrush(Qt::lightGray);
                chartPrice->setTitle(symbol + " — Price");
                chartPrice->addSeries(candleSeries);

                auto *axisYPrice = new QValueAxis();
                axisYPrice->setTitleText("Price");
                axisYPrice->setLabelFormat("%.2f");

                chartPrice->addAxis(axisXprice, Qt::AlignBottom);
                chartPrice->addAxis(axisYPrice, Qt::AlignLeft);
                candleSeries->attachAxis(axisXprice);
                candleSeries->attachAxis(axisYPrice);

                // 4) Log‐scale price chart
                chartPriceLog->setBackgroundBrush(Qt::lightGray);
                chartPriceLog->setTitle(symbol + " — Price (log)");
                chartPriceLog->addSeries(candleSeriesLog);

                auto *axisYLog = new QLogValueAxis();
                axisYLog->setTitleText("Price (log)");
                axisYLog->setBase(5);
                axisYLog->setLabelFormat("%.2f");
                axisYLog->setMinorTickCount(9);
                //axisYLog->setRange(minVal, maxVal);

                chartPriceLog->addAxis(axisXlog, Qt::AlignBottom);
                chartPriceLog->addAxis(axisYLog, Qt::AlignLeft);
                candleSeriesLog->attachAxis(axisXlog);
                candleSeriesLog->attachAxis(axisYLog);

                // !olume chart
                auto *volSeries = new QLineSeries();
                volSeries->setName("Vol");
                for (auto &dt : times)
                {
                    qint64 ts = dt.toMSecsSinceEpoch();
                    volSeries->append(ts, volMap[dt]);
                }

                // 2) add it to the chart
                chartVol->addSeries(volSeries);

                // 3) attach your datetime axis
                axisXvol->setRange(dateTimeStart, dateTimeEnd);
                chartVol->addAxis(axisXvol, Qt::AlignBottom);
                volSeries->attachAxis(axisXvol);

                // 4) Y axis stays the same
                auto *axisYVol = new QValueAxis();
                axisYVol->setTitleText("Volume");
                axisYVol->setGridLineVisible(false);
                chartVol->addAxis(axisYVol, Qt::AlignLeft);
                volSeries->attachAxis(axisYVol);


            }
        }
        else
        {
            QDateTime nullDateTime{QDate{2000, 1, 1}, QTime{0, 0, 0}};
            ui->dateTimeEditFrom->setDateTime(nullDateTime);
            ui->dateTimeEditTo->setDateTime(nullDateTime);
        }

        // 8) Affectation au QChartView
        ui->chartViewVolume->setChart(chartVol);
        ui->chartViewVolume->setRenderHint(QPainter::Antialiasing);
        ui->chartViewPriceLog->setChart(chartPriceLog);
        ui->chartViewPriceLog->setRenderHint(QPainter::Antialiasing);
        ui->chartViewPrice->setChart(chartPrice);
        ui->chartViewPrice->setRenderHint(QPainter::Antialiasing);
    }
    else if (deselected.size() > 0)
    {
        for (auto chartView : getChartViews())
        {
            chartView->setChart(new QChart{});
            chartView->chart()->setBackgroundBrush(
                        Qt::lightGray);
        }
    }
    for (auto curChart : currentCharts)
    {
        if (curChart != nullptr)
        {
            curChart->deleteLater();
        }
    }
}

void PaneData::run(bool start)
{
    for (auto &connection : m_connectionStreamers)
    {
        disconnect(connection);
    }
    m_connectionStreamers.clear();
    if (start)
    {
        m_job->start();
        ui->listViewTemplates->setEnabled(false);
        auto selVariables = getTradingPairsSelected()->getSelectedVariables();
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
            m_connectionStreamers
                    << connect(firstVariableAvailable.streamReader,
                               &StreamReaderAbstract::message,
                               this,
                               &PaneData::writeMessage,
                               Qt::QueuedConnection);
            m_connectionStreamers
                    << connect(firstVariableAvailable.streamReader,
                               &StreamReaderAbstract::messageError,
                               this,
                               &PaneData::writeMessageError,
                               Qt::QueuedConnection);

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
