#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QModelIndex>
#include <QFile>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QTreeWidgetItem>
#include "filedownloader.h"
#include "epidownloader.h"
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
    AboutDialog *m_aboutDialog;
    QGraphicsScene *scene;
    Ui::MainWindow *ui;
    FileDownloader *downloader;
    EpiDownloader *epidownloader;
    EpiParser *epis;
    Episode selectedEpisode;
    QNetworkAccessManager downloadHttp;
    QFile downloadFile;
    QNetworkReply *downloadReply;

    QTreeWidgetItem *catEpisodes;
    QTreeWidgetItem *catSpecials;
    QTreeWidgetItem *catTrailerDemo;
    QTreeWidgetItem *catTrash;

    void readingReadyBytes();
    void setNoimage();
    void noEpiSelected();
    void setControlsDownloading();
    void setControlsDownloadFinished();


private slots:
    void updateData();
    void aboutMMMLauncher();
    void destroyAboutDialog();
    void episodeSelected();
    /*void episodeSelected(QModelIndex row);*/
    void downloadEpisode();
    void finishedDownload();
    void downloadProgress(qint64 bytesReceived, qint64 bytesToal/*, double speed, QString unit*/);
};

#endif // MAINWINDOW_H
