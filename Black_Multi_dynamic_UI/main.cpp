#include "mainwidget.h"
#include "meanspackinstaller.h"
#include "meanssingleapplication.h"
#include <QApplication>
#include <QCoreApplication>
#include <QDir>
#include <QLabel>
#include <QSharedMemory>

int main(int argc, char *argv[])
{
    QCoreApplication::addLibraryPath("../plugin");

    SingleApplication app(argc, argv, "BLACK_MULTI");
    if(argc!=1)
    {
        MeansPackInstaller install(QString::fromLocal8Bit(argv[1]));
        if(app.isRunning())
        {
            app.sendMessage("1");
            return 0;
        }
    }

    if(app.isRunning())
        return 0;
    MainWidget *w=new MainWidget;
    QObject::connect(&app,SIGNAL(messageAvailable(QString)),w,SIGNAL(refresh()));
    w->show();

    return app.exec();
}
