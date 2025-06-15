#include <QInputDialog>
#include <QMessageBox>

#include "model/TemplateManager.h"
#include "model/TradingPairs.h"
#include "model/Functions.h"
#include "model/Indicators.h"
#include "model/TemplateParams.h"
#include "model/pairs/TickDelegate.h"

#include "PaneTemplates.h"
#include "ui_PaneTemplates.h"

PaneTemplates::PaneTemplates(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PaneTemplates)
{
    ui->setupUi(this);
    ui->listViewTemplates->setModel(TemplateManager::instance());
    _connectSlots();
    ui->listViewTemplates->setCurrentIndex(
        TemplateManager::instance()->index(0, 0));
    ui->tableViewParams->setItemDelegate(
        new TickDelegate{ui->listViewTemplates});
}

void PaneTemplates::_connectSlots()
{
    connect(ui->buttonAdd,
            &QPushButton::clicked,
            this,
            &PaneTemplates::addTemplate);
    connect(ui->buttonRemove,
            &QPushButton::clicked,
            this,
            &PaneTemplates::removeTemplate);
    connect(ui->listViewTemplates->selectionModel(),
            &QItemSelectionModel::selectionChanged,
            this,
            &PaneTemplates::onTemplateSelected);
    connect(ui->lineEditFilterPairs,
            &QLineEdit::textEdited,
            this,
            &PaneTemplates::filterPairs);
}

PaneTemplates::~PaneTemplates()
{
    delete ui;
}

void PaneTemplates::addTemplate()
{
    const QString &name = QInputDialog::getText(
                this,
                tr("Template name"),
                tr("Enter the template name"));
    if (!name.isEmpty())
    {
        TemplateManager::instance()->addTemplate(name);
    }
}

void PaneTemplates::removeTemplate()
{
    const auto &selIndexes = ui->listViewTemplates->selectionModel()->selectedIndexes();
    if (selIndexes.size() > 0)
    {
        const auto &firstIndex = selIndexes.first();
        if (firstIndex.row() > 0)
        {
            TemplateManager::instance()->removeTemplate(firstIndex);
        }
        else
        {
            QMessageBox::information(this,
                                     tr("Removing failed"),
                                     tr("You can’t remove the default template"));
        }
    }
    else
    {
        QMessageBox::information(this,
                                 tr("No selection"),
                                 tr("You need to select a template to remove"));
    }
}

void PaneTemplates::onTemplateSelected(
        const QItemSelection &selected, const QItemSelection &deselected)
{
    ui->lineEditFilterPairs->clear();
    if (selected.size() > 0 || deselected.size() > 0)
    {
        auto model = ui->tableViewPairs->model();
        if (model != nullptr)
        {
            ui->tableViewPairs->setModel(nullptr);
            model->deleteLater();
        }
        if (selected.size() > 0)
        {
            auto firstIndex = selected.indexes().first();
            const auto &templateId = TemplateManager::instance()->getId(firstIndex);

            ui->tableViewPairs->setModel(TradingPairs::instance(templateId));
            QHeaderView* header = ui->tableViewPairs->horizontalHeader();
            header->setSectionsClickable(true);

            auto functionsModel = new Functions{templateId, ui->listViewPairTransformations};
            ui->listViewPairTransformations->setModel(functionsModel);

            ui->listViewIndicators->setModel(Indicators::instance(templateId));

            ui->tableViewParams->setModel(TemplateParams::instance(templateId));
        }
    }
}

void PaneTemplates::filterPairs(const QString &text)
{
    auto model = ui->tableViewPairs->model();
    if (model != nullptr)
    {
        int nRows = model->rowCount();
        for (int i=0; i<nRows; ++i)
        {
            const QString &pairName = model->index(i, 0).data().toString();
            bool toShow = pairName.contains(text, Qt::CaseInsensitive);
            ui->tableViewPairs->setRowHidden(i, !toShow);
        }
    }
}

