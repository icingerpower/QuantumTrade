#ifndef PANESIMULATION_H
#define PANESIMULATION_H

#include <QWidget>

namespace Ui {
class PaneSimulation;
}

class PaneSimulation : public QWidget
{
    Q_OBJECT

public:
    explicit PaneSimulation(QWidget *parent = nullptr);
    ~PaneSimulation();

private:
    Ui::PaneSimulation *ui;
};

#endif // PANESIMULATION_H
