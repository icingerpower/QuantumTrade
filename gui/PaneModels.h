#ifndef PANEMODELS_H
#define PANEMODELS_H

#include <QWidget>

namespace Ui {
class PaneModels;
}

class PaneModels : public QWidget
{
    Q_OBJECT

public:
    explicit PaneModels(QWidget *parent = nullptr);
    ~PaneModels();

private:
    Ui::PaneModels *ui;
};

#endif // PANEMODELS_H
