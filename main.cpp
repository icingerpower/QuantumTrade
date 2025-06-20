#include <QApplication>

#include "../common/workingdirectory/WorkingDirectoryManager.h"
#include "../common/workingdirectory/DialogOpenConfig.h"
#include "../common/types/types.h"
//#include "model/pairs/Tick.h"

#include "model/pairs/VariableAbstract.h"

#include "gui/MainWindow.h"

int main(int argc, char *argv[])
{
    qRegisterMetaType<QList<QStringList>>();
    qRegisterMetaType<QHash<QString, QSet<QString>>>();
    qRegisterMetaType<QHash<QString, QVariant>>();
    qRegisterMetaType<QHash<QString, QHash<QString, QVariant>>>();
    qRegisterMetaType<QSet<QString>>();

    QApplication a(argc, argv);
    WorkingDirectoryManager::instance()->installDarkOrangePalette();
    DialogOpenConfig dialog;
    dialog.exec();
    if (dialog.wasRejected())
    {
        return 0;
    }
    VariableAbstract::setDatabaseFolder(
        WorkingDirectoryManager::instance()->workingDir().path());
    MainWindow w;
    w.show();
    return a.exec();
}


