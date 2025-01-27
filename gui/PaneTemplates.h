#ifndef PANETEMPLATES_H
#define PANETEMPLATES_H

#include <QWidget>

namespace Ui {
class PaneTemplates;
}

class PaneTemplates : public QWidget
{
    Q_OBJECT

public:
    explicit PaneTemplates(QWidget *parent = nullptr);
    ~PaneTemplates();

private:
    Ui::PaneTemplates *ui;
};

#endif // PANETEMPLATES_H
