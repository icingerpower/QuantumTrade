#ifndef PANEDATA_H
#define PANEDATA_H

#include <QWidget>
#include <QItemSelection>

namespace Ui {
class PaneData;
}

class PaneData : public QWidget
{
    Q_OBJECT

public:
    explicit PaneData(QWidget *parent = nullptr);
    ~PaneData();

private slots:
    void onTemplateSelected(
            const QItemSelection &selected, const QItemSelection &deselected);
    void run(bool start);

private:
    Ui::PaneData *ui;
    void _connectSlots();
};

#endif // PANEDATA_H
