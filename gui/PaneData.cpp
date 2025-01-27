#include "PaneData.h"
#include "ui_PaneData.h"

PaneData::PaneData(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PaneData)
{
    ui->setupUi(this);
}

PaneData::~PaneData()
{
    delete ui;
}
