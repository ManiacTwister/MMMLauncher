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
    app.setOrganizationDomain("http://maniactwister.de/");

    QTranslator qtTranslator;
    qtTranslator.load("qt_" + QLocale::system().name(), QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    app.installTranslator(&qtTranslator);
    QTranslator myappTranslator;
    myappTranslator.load("myapp_" + QLocale::system().name());
    app.installTranslator(&myappTranslator);

    MainWindow w;
    w.show();
    
    return app.exec();
}
