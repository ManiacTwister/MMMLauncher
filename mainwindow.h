#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QModelIndex>
#include "filedownloader.h"
#include "aboutdialog.h"
#include "epiparser.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    bool init();
    
private:
    QGraphicsScene *scene;
    Ui::MainWindow *ui;
    FileDownloader *downloader;
    AboutDialog *m_aboutDialog;
    EpiParser *epis;

private slots:
    void updateData();
    void aboutMMMLauncher();
    void destroyAboutDialog();
    void episodeSelected(QModelIndex row);
};

#endif // MAINWINDOW_H
