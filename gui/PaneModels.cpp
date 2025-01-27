#include "PaneModels.h"
#include "ui_PaneModels.h"

PaneModels::PaneModels(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PaneModels)
{
    ui->setupUi(this);
}

PaneModels::~PaneModels()
{
    delete ui;
}
