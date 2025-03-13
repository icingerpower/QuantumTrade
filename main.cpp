#include <QApplication>

#include "../common/workingdirectory/WorkingDirectoryManager.h"
#include "../common/workingdirectory/DialogOpenConfig.h"
#include "../common/types/types.h"

#include "gui/MainWindow.h"

int main(int argc, char *argv[])
{
    qRegisterMetaType<QList<QStringList>>();
    qRegisterMetaType<QHash<QString, QSet<QString>>>();
    qRegisterMetaType<QHash<QString, QVariant>>();

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


