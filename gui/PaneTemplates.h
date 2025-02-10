#ifndef PANETEMPLATES_H
#define PANETEMPLATES_H

#include <QWidget>
#include <QItemSelection>

namespace Ui {
class PaneTemplates;
}

class PaneTemplates : public QWidget
{
    Q_OBJECT

public:
    explicit PaneTemplates(QWidget *parent = nullptr);
    ~PaneTemplates();

public slots:
    void addTemplate();
    void removeTemplate();
    void onTemplateSelected(
            const QItemSelection &selected, const QItemSelection &deselected);
    void filterPairs(const QString &text);

private:
    Ui::PaneTemplates *ui;
    void _connectSlots();
};

#endif // PANETEMPLATES_H
