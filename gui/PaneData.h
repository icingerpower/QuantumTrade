#ifndef PANEDATA_H
#define PANEDATA_H

#include <QWidget>
#include <QItemSelection>

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

private slots:
    void onTemplateSelected(
            const QItemSelection &selected, const QItemSelection &deselected);
    void run(bool start);

private:
    Ui::PaneData *ui;
    void _connectSlots();
    QSharedPointer<Job> m_job;
};

#endif // PANEDATA_H
