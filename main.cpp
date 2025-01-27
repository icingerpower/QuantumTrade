#include <QApplication>

#include "../common/workingdirectory/WorkingDirectoryManager.h"
#include "../common/workingdirectory/DialogOpenConfig.h"

#include "gui/MainWindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    WorkingDirectoryManager::instance()->installDarkOrangePalette();
    DialogOpenConfig dialog;
    dialog.exec();
    if (dialog.wasRejected())
    {
        return 0;
    }
    MainWindow w;
    w.show();
    return a.exec();
}


