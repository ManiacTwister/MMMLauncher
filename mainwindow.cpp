#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "filedownloader.h"
#include "epidownloader.h"
#include "aboutdialog.h"
#include "episode.h"
#include "category.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>
#include <QString>
#include <QTableWidgetItem>
#include <QMessageBox>
#include <QGraphicsScene>
#include <QModelIndex>
#include <QFile>
#include <QFileInfo>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    m_aboutDialog(0),
    ui(new Ui::MainWindow)
{
    //QStringList list;
    QUrl updateUrl("http://launcher.maniactwister.de/update/");
    downloader = new FileDownloader(updateUrl, this);
    connect(downloader, SIGNAL(downloaded()), SLOT(updateData()));

    ui->setupUi(this);

    connect(ui->buttonAbout, SIGNAL(clicked()), SLOT(aboutMMMLauncher()));
    connect(ui->buttonDownload, SIGNAL(clicked()), SLOT(downloadEpisode()));
    connect(ui->treeWidget, SIGNAL(itemSelectionChanged()), SLOT(episodeSelected()));

    /* TABLE VERSION BACKUP */
    //QItemSelectionModel *sm = ui->tableWidget->selectionModel();
    //connect(sm, SIGNAL(currentRowChanged(QModelIndex,QModelIndex)), SLOT(episodeSelected(QModelIndex)));


    noEpiSelected();

    // epiDescription background
    ui->epiDescription->viewport()->setAutoFillBackground(false);

    /* TABLE VERSION BACKUP */
    // Setup table layout
    /*ui->tableWidget->setRowCount(0);
    ui->tableWidget->setColumnCount(2);

    //add header to QTableWidgetItem
    list<<"Titel" << "Autor";
    ui->tableWidget->setHorizontalHeaderLabels(list);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Interactive);
    ui->tableWidget->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);
    ui->tableWidget->verticalHeader()->setDefaultSectionSize(20);

    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);*/


    catEpisodes     =    new QTreeWidgetItem(ui->treeWidget);
    catSpecials     =    new QTreeWidgetItem(ui->treeWidget);
    catTrailerDemo  =    new QTreeWidgetItem(ui->treeWidget);
    catTrash        =    new QTreeWidgetItem(ui->treeWidget);
    catEpisodes->setText(0, "Episoden");
    catSpecials->setText(0, "Specials");
    catTrailerDemo->setText(0, "Trailer & Demos");
    catTrash->setText(0, "Trash");
    catEpisodes->setFlags(Qt::ItemIsEnabled);
    catSpecials->setFlags(Qt::ItemIsEnabled);
    catTrailerDemo->setFlags(Qt::ItemIsEnabled);
    catTrash->setFlags(Qt::ItemIsEnabled);

    ui->treeWidget->expandAll();

    ui->treeWidget->header()->setSectionResizeMode(QHeaderView::Stretch);
    ui->treeWidget->header()->setSectionResizeMode(1, QHeaderView::Interactive);
}

bool MainWindow::init()
{
    return true;
}

void MainWindow::episodeSelected()
{
    if(ui->treeWidget->selectedItems().count() < 1) {
        noEpiSelected();
        return;
    }


    QTreeWidgetItem* item = ui->treeWidget->selectedItems()[0];
    qDebug() << "Test";
    selectedEpisode = epis->episodes[item->data(0,Qt::UserRole).toInt()];
    QString image = "/home/maniactwister/Projects/Qt/MMMLauncher/images/Screenshots/"+ selectedEpisode.getImage();
    QFile file(image);

    if(file.exists()) {
        scene = new QGraphicsScene();
        scene->addPixmap(QPixmap(image));
        ui->epiScreenshoot->setScene(scene);
    } else {
        setNoimage();
    }
    file.close();
    ui->epiDescription->setPlainText(selectedEpisode.getDescription());

    // TODO: Check if epi is downloaded and display the correct buttons
    ui->buttonDownload->setDisabled(false);
}

/* TABLE VERSION BACKUP */
/*void MainWindow::episodeSelected(QModelIndex row)
{
    selectedEpisode = epis->episodes[row.row()];
    QString image = "/home/maniactwister/Projects/Qt/MMMLauncher/images/Screenshots/"+ selectedEpisode.getImage();
    QFile file(image);

    if(file.exists()) {
        scene = new QGraphicsScene();
        scene->addPixmap(QPixmap(image));
        ui->epiScreenshoot->setScene(scene);
    } else {
        setNoimage();
    }
    file.close();
    ui->epiDescription->setPlainText(selectedEpisode.getDescription());
}*/

void MainWindow::setNoimage() {
    scene = new QGraphicsScene();
    scene->addPixmap(QPixmap(":/images/images/noimage.png"));
    ui->epiScreenshoot->setScene(scene);
}

void MainWindow::noEpiSelected() {
    setNoimage();
    ui->epiDescription->setPlainText("");
    ui->buttonDownload->setDisabled(true);
    ui->buttonSetup->setDisabled(true);
    ui->buttonStart->setDisabled(true);
    return;
}

void MainWindow::setControlsDownloading()
{
    ui->buttonDownload->setDisabled(true);
    ui->buttonSetup->setDisabled(true);
    ui->buttonStart->setDisabled(true);
    ui->buttonDownloadall->setDisabled(true);
    ui->buttonUpdate->setDisabled(true);

    ui->progressBar->setDisabled(false);
    ui->labelUpdate->setText("Downloading ...");
    ui->treeWidget->setDisabled(true);
}

void MainWindow::setControlsDownloadFinished()
{
    episodeSelected();
    ui->buttonDownloadall->setDisabled(false);
    ui->buttonUpdate->setDisabled(false);

    ui->progressBar->setDisabled(true);
    ui->progressBar->setValue(0);
    ui->labelUpdate->setText("Für ein Update auf \"Update\" klicken!");
    ui->treeWidget->setDisabled(false);
}

void MainWindow::updateData()
{
    epis = new EpiParser(downloader->downloadedData());
    epis->parse();
    int i=0;
    for (auto &episode : epis->episodes) {
        QTreeWidgetItem *item;
        /*int row = ui->tableWidget->model()->rowCount();
        ui->tableWidget->model()->insertRow(row);
        ui->tableWidget->setItem(row, 0, new QTableWidgetItem(episode.getCategoryIcon(), episode.getTitle()));
        ui->tableWidget->setItem(row, 1, new QTableWidgetItem(episode.getAuthor()));*/
        if(episode.getCategory() == Category::Episode) {
            item = new QTreeWidgetItem(catEpisodes);
        } else if(episode.getCategory() == Category::Special) {
            item = new QTreeWidgetItem(catSpecials);
        } else if(episode.getCategory() == Category::TrailerDemo) {
            item = new QTreeWidgetItem(catTrailerDemo);
        } else if(episode.getCategory() == Category::Trash) {
            item = new QTreeWidgetItem(catTrash);
        }

        item->setIcon(0, episode.getCategoryIcon());
        item->setText(0, episode.getTitle());
        item->setText(1, episode.getAuthor());
        item->setData(0,Qt::UserRole, i); // unique ID HERE;
        qDebug() << episode.getTitle();
        i++;
    }
}

void MainWindow::downloadEpisode()
{
    /*QUrl url(selectedEpisode.getDownloadUrl());

    QFileInfo fileInfo(url.path());
    QString strhost=url.encodedHost();
    QString filename=fileInfo.fileName();
    file.setFileName(QDir::currentPath()+ "/" +);
    http.setHost(url.host(),url.port(80));
    http.get(url.path(),&file);

    if(!file.open(QIODevice::WriteOnly))
    {
    QMessageBox::warning(NULL,"warning","file is not opened",QMessageBox::Ok);
    }
    file.write(http.readAll());
    http.close();
    return true;


    const QNetworkRequest& request = QNetworkRequestdlurl;
    reply = http->get(request);
    connect(reply, SIGNAL(readyRead()), this, SLOT( readingReadyBytes()));
    downloadFile = new QFile(QDir::currentPath()+ "/" + reply->rawHeader("Content-Disposition"));*/

    /*QUrl url(selectedEpisode.getDownloadUrl());
    const QNetworkRequest& request = QNetworkRequest(url);
    downloadReply = downloadHttp->get(request);
    downloadHttp->g
    connect(downloadReply, SIGNAL(readyRead()), this, SLOT(readingReadyBytes()));
    connect(downloadReply, SIGNAL(finished()), this, SLOT(finishedDownload()));
    downloadFile = new QFile(QDir::currentPath()+ "/" + downloadReply->rawHeader("Content-Disposition"));*/
    qDebug() << QString("Downloading");
    setControlsDownloading();
    epidownloader = new EpiDownloader(selectedEpisode.getDownloadUrl(), this);
    connect(epidownloader, SIGNAL(downloaded()), SLOT(finishedDownload()));
    connect(epidownloader, SIGNAL(downloadProgress(qint64, qint64, double, QString)), SLOT(downloadProgress(qint64, qint64/*, double, QString*/)));


}

void MainWindow::readingReadyBytes()
{
    //downloadFile->write(downloadReply->read(downloadReply->bytesAvailable()));
}

void MainWindow::downloadProgress(qint64 bytesReceived, qint64 bytesTotal/*, double speed, QString unit*/)
{
    double progress = ((double) bytesReceived / (double) bytesTotal) * 100;
    ui->progressBar->setValue(progress);
    //ui->progressBar->setMessage(QString::fromLatin1("%1 %2").arg(speed, 3, 'f', 1).arg(unit));
}

void MainWindow::finishedDownload()
{
    QFileInfo fileInf(epidownloader->filename); //maybe url.toString() might also be good
    QString fileName = fileInf.fileName();

    QFile file(fileName);
    file.open(QIODevice::WriteOnly);
    QDataStream out(&file);
    out << epidownloader->downloadedData();

    setControlsDownloadFinished();
    qDebug() << QString("Downloaded" + fileName);
}

void MainWindow::aboutMMMLauncher()
{
    if (!m_aboutDialog) {
        m_aboutDialog = new AboutDialog(this);
        connect(m_aboutDialog, SIGNAL(finished(int)), this, SLOT(destroyAboutDialog()));
    }
    m_aboutDialog->show();
}

void MainWindow::destroyAboutDialog()
{
    if (m_aboutDialog) {
        m_aboutDialog->deleteLater();
        m_aboutDialog = 0;
    }
}


MainWindow::~MainWindow()
{
    delete ui;
}
