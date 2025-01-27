#include "PaneSimulation.h"
#include "ui_PaneSimulation.h"

PaneSimulation::PaneSimulation(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PaneSimulation)
{
    ui->setupUi(this);
}

PaneSimulation::~PaneSimulation()
{
    delete ui;
}
