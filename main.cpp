#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setApplicationVersion(APPLICATION_VERSION);
    a.setApplicationName("MMMLauncher");
    a.setOrganizationName("ManiacTwister");
    a.setOrganizationDomain("http://maniactwister.de/");
    MainWindow w;
    w.show();
    
    return a.exec();
}
