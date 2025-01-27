#ifndef PANEDATA_H
#define PANEDATA_H

#include <QWidget>

namespace Ui {
class PaneData;
}

class PaneData : public QWidget
{
    Q_OBJECT

public:
    explicit PaneData(QWidget *parent = nullptr);
    ~PaneData();

private:
    Ui::PaneData *ui;
};

#endif // PANEDATA_H
