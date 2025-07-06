#ifndef PANEDATA_H
#define PANEDATA_H

#include <QWidget>
#include <QItemSelection>
#include <QtCharts/QChartView>

#include "model/readers/Job.h"

namespace Ui {
class PaneData;
}

class TradingPairsSelected;
class TemplateParamsSelected;

class PaneData : public QWidget
{
    Q_OBJECT

public:
    explicit PaneData(QWidget *parent = nullptr);
    ~PaneData();
    TradingPairsSelected *getTradingPairsSelected() const;
    TemplateParamsSelected *getTemplateParamsSelected() const;

public slots:
    void writeMessage(const QString &message);
    void writeMessageError(const QString &message);

private slots:
    void onTemplateSelected(
            const QItemSelection &selected, const QItemSelection &deselected);
    void onPairSelected(
            const QItemSelection &selected, const QItemSelection &deselected);
    void run(bool start);

private:
    QList<QChartView *> getChartViews();
    Ui::PaneData *ui;
    void _connectSlots();
    QSharedPointer<Job> m_job;
    QMetaObject::Connection m_connectionPair;
    QList<QMetaObject::Connection> m_connectionStreamers;
    void _writeMessage(const QString &message, const QColor &color);
};

#endif // PANEDATA_H
