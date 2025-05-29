#include "model/ApiParams.h"

#include "PaneSettings.h"
#include "ui_PaneSettings.h"

PaneSettings::PaneSettings(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PaneSettings)
{
    ui->setupUi(this);
    ui->tableViewApi->setModel(ApiParams::instance());
}

PaneSettings::~PaneSettings()
{
    delete ui;
}
