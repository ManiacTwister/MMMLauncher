#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "filedownloader.h"
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

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    m_aboutDialog(0),
    ui(new Ui::MainWindow)
{
    QUrl updateUrl("http://launcher.maniactwister.de/update/");
    downloader = new FileDownloader(updateUrl, this);
    connect(downloader, SIGNAL(downloaded()), SLOT(updateData()));

    ui->setupUi(this);
    connect(ui->buttonAbout, SIGNAL(clicked()), SLOT(aboutMMMLauncher()));

    // Setup table layout
    QStringList list;
    //set size of QTableWidget
    ui->tableWidget->setRowCount(0);
    ui->tableWidget->setColumnCount(2);

    //add header to QTableWidgetItem
    list<<"Titel" << "Autor";
    ui->tableWidget->setHorizontalHeaderLabels(list);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Interactive);
    ui->tableWidget->verticalHeader()->setDefaultSectionSize(20);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    scene = new QGraphicsScene();
    scene->addPixmap(QPixmap(":/images/images/noimage.png"));
    ui->epiScreenshoot->setScene(scene);
    QItemSelectionModel *sm = ui->tableWidget->selectionModel();
    connect(sm, SIGNAL(currentRowChanged(QModelIndex,QModelIndex)), SLOT(episodeSelected(QModelIndex)));
}

bool MainWindow::init()
{
    return true;
}

void MainWindow::episodeSelected(QModelIndex row)
{
    Episode epi = epis->episodes[row.row()];
    scene = new QGraphicsScene();
    scene->addPixmap(QPixmap("/home/maniactwister/Projects/Qt/MMMLauncher/images/Screenshots/"+ epi.getImage()));
    ui->epiScreenshoot->setScene(scene);
    ui->epiDescription->setText(epi.getDescription());
}

void MainWindow::updateData()
{
    epis = new EpiParser(downloader->downloadedData());
    epis->parse();

    for (auto &episode : epis->episodes) {
        int row = ui->tableWidget->model()->rowCount();
        ui->tableWidget->model()->insertRow(row);
        ui->tableWidget->setItem(row, 0, new QTableWidgetItem(episode.getCategoryIcon(), episode.getTitle()));
        ui->tableWidget->setItem(row, 1, new QTableWidgetItem(episode.getAuthor()));
        qDebug() << episode.getTitle();
    }

}

void MainWindow::aboutMMMLauncher()
{
    if (!m_aboutDialog) {
        m_aboutDialog = new AboutDialog(this);
        connect(m_aboutDialog, SIGNAL(finished(int)),
                this, SLOT(destroyAboutDialog()));
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
