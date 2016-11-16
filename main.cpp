#include "mainwindow.h"
#include <QApplication>
#include <QLibraryInfo>
#include <QLocale>
#include <QTranslator>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setApplicationVersion(APPLICATION_VERSION);
    app.setApplicationName("MMMLauncher");
    app.setOrganizationName("ManiacTwister");
    app.setOrganizationDomain("https://maniactwister.de/");


    MainWindow w;
    w.show();
    
    return app.exec();
}
