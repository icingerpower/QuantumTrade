#include "PaneTemplates.h"
#include "ui_PaneTemplates.h"

PaneTemplates::PaneTemplates(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PaneTemplates)
{
    ui->setupUi(this);
}

PaneTemplates::~PaneTemplates()
{
    delete ui;
}
