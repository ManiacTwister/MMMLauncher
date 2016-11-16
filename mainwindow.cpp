#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "filedownloader.h"
#include "epidownloader.h"
#include "aboutdialog.h"
#include "Models/episode.h"
#include "Models/category.h"
#include "Models/author.h"
#include "qextract.h"
#include "version.h"
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
#include <QDir>
#include <QSettings>
#include <QToolButton>
#include <QFont>
#include "newtabdialog.h"
#include <QIODevice>
#include <QDesktopServices>
#include <QFileDialog>
#include <QDialogButtonBox>
#include "desktop.h"

/**
  *  Create mainwindow
  *
  *  @param QWidget *parent
  *  @return void
  *
**/
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    m_aboutDialog(0),
    ui(new Ui::MainWindow),
    networkManager(this)
{
    settings = new QSettings(/*settings->value("baseDir").toString() + "/.config.ini", */QSettings::IniFormat, QSettings::UserScope, "MMMLauncher");

    if(settings->value("baseDir").isNull() || settings->value("baseDir").toString().isEmpty() || !QDir(settings->value("baseDir").toString()).exists()) {
        QMessageBox::information(this, "Setup", tr("Bevor der Launcher benutzt werden kann, muss ein Ordner ausgewählt werden, in dem heruntergeladene Episoden gespeichert werden sollen."));

        QString destination;
        while(!QDir(destination).exists() || destination.isEmpty()) {
            destination = QFileDialog::getExistingDirectory(this, "Select destionation folder", ".",
                                                                            QFileDialog::ShowDirsOnly
                                                                            | QFileDialog::DontResolveSymlinks);
        }

        settings->setValue("baseDir", destination);
    }

    if(settings->value("aktionsliste").isNull()) {
        settings->setValue("aktionsliste", true);
    }
    updateSettings();

    //QStringList list;
    downloader = new FileDownloader(QUrl(settings->value("updateUrl").toString()), this);
    connect(downloader, SIGNAL(downloaded()), SLOT(checkIfUpdateNeeded()));

    ui->setupUi(this);

    connect(ui->buttonAbout, SIGNAL(clicked()), SLOT(aboutMMMLauncher()));
    connect(ui->buttonDownload, SIGNAL(clicked()), SLOT(downloadEpisode()));
    connect(ui->buttonDownloadall, SIGNAL(clicked()), SLOT(downloadAll()));
    connect(ui->buttonStart, SIGNAL(clicked()), SLOT(startEpisode()));
    connect(ui->buttonSetup, SIGNAL(clicked()), SLOT(setupEpisode()));
    connect(ui->buttonUpdate, SIGNAL(clicked()), SLOT(restartApp())); // Dirty but easy.. restart app and update jsons
    connect(ui->treeWidget, SIGNAL(itemSelectionChanged()), SLOT(episodeSelected()));
    connect(ui->tabWidget, SIGNAL(currentChanged(int)), this, SLOT(onTabChanged()));
    connect(ui->tabWidget, SIGNAL(tabCloseRequested(int)), this, SLOT(closeTab(int)));
    connect(ui->directoryContent, SIGNAL(itemDoubleClicked(QTreeWidgetItem*, int)), SLOT(openAdditionalFile(QTreeWidgetItem*)));

    noEpiSelected();

    // epiDescription background
    ui->epiDescription->viewport()->setAutoFillBackground(false);

    // New tab
    QToolButton *newTabButton = new QToolButton(this);
    ui->tabWidget->setCornerWidget(newTabButton);
    newTabButton->setCursor(Qt::ArrowCursor);
    newTabButton->setAutoRaise(true);
    newTabButton->setIcon(QIcon(":/icons/images/plus.png"));
    connect(newTabButton, SIGNAL(clicked()), this, SLOT(newTabButton()));
    newTabButton->setToolTip(tr("Seite hinzufügen"));

    setupTreewidget(ui->treeWidget);
    setCurrentTreeWidget();

    // Remove close button from frist tab and resize it (width only)
    int tabheight = ui->tabWidget->tabBar()->tabButton(0, QTabBar::RightSide)->geometry().height();
    ui->tabWidget->tabBar()
            ->tabButton(0, QTabBar::RightSide)
            ->resize(0, tabheight);
    ui->tabWidget->setTabText(0, tr("Übersicht"));

    connect(ui->actionSettings, SIGNAL(triggered()), this, SLOT(settingsDialog()));
    connect(ui->actionQuit, SIGNAL(triggered()), qApp, SLOT(quit()));
    connect(ui->actionAbout, SIGNAL(triggered()), this, SLOT(aboutMMMLauncher()));

    createLanguageMenu();

    QIcon::setThemeName(Desktop::determineIconTheme());
    ui->actionSettings->setIcon(QIcon::fromTheme("preferences-system"));
    ui->actionQuit->setIcon(QIcon::fromTheme("application-exit"));
    ui->actionAbout->setIcon(QIcon::fromTheme("help-about"));

    if(!settings->value("aktionsliste").toBool()) {
        ui->actions->hide();
        ui->length->hide();
        ui->difficulty->hide();
    }
}

void MainWindow::updateSettings()
{
    settings->setValue("apiUrl",        QUrl("http://api.launcher.maniactwister.de/v3"));
    settings->setValue("updateUrl",     QUrl("http://launcher.maniactwister.de/lastupdate"));
    settings->setValue("epiDir",        settings->value("baseDir").toString() + "/" + "Episoden");
    settings->setValue("screenDir",     settings->value("baseDir").toString() + "/" + "Screenshots");
    settings->setValue("epiJson",       settings->value("baseDir").toString() + "/" + ".episoden.json");
    settings->setValue("categoryJson",  settings->value("baseDir").toString() + "/" + ".categories.json");
    settings->setValue("authorJson",    settings->value("baseDir").toString() + "/" + ".authors.json");
    settings->sync();
    epiDir = new QDir(settings->value("epiDir").toString());
    screenDir = new QDir(settings->value("screenDir").toString());
}

/**
  *  Set properties for treewidgets
  *
  *  @param TreeWidget* widget
  *  @return void
  *
**/
void MainWindow::setupTreewidget(QTreeWidget* widget)
{
    widget->expandAll();

    widget->header()->setSectionResizeMode(QHeaderView::Stretch);
    widget->header()->setSectionResizeMode(1, QHeaderView::Interactive);
}

/**
  *  Signal - fired when tab changed
  *
  *  @param int index
  *  @return void
  *
**/
void MainWindow::onTabChanged()
{
    if(currentTreeWidget->selectedItems().count() > 0) {
        currentTreeWidget->selectedItems()[0]->setSelected(false);
        noEpiSelected();
    }

    setCurrentTreeWidget();
}

/**
  *  Set the current treewidget (for the visible tab)
  *
  *  @return void
  *
**/
void MainWindow::setCurrentTreeWidget()
{
    if(ui->tabWidget->currentIndex() == 0) {
        currentTreeWidget = ui->treeWidget;
    } else {
        currentTreeWidget = (QTreeWidget*) ui->tabWidget->currentWidget();
    }

}

/**
  *  Slot - fired when a item in treewidget selected
  *
  *  @return void
  *
**/
void MainWindow::episodeSelected()
{
    if(currentTreeWidget->selectedItems().count() < 1) {
        noEpiSelected();
        return;
    }

    QTreeWidgetItem* item = currentTreeWidget->selectedItems()[0];
    selectedEpisode = epis->episodes.find(item->data(0,Qt::UserRole).toInt()).value();

    setImage();

    ui->epiDescription->setPlainText(selectedEpisode->getDescription());
    QDir *selectedEpiDir = new QDir(epiDir->absolutePath() + "/" + selectedEpisode->getDirectory());

    ui->actions->setText(tr("<b>Aktionen:</b>") + " " + selectedEpisode->getActionsString());
    ui->length->setText(tr("<b>Länge:</b>") + " " + selectedEpisode->getLengthString());
    ui->difficulty->setText(tr("<b>Schwierigkeit:</b>") + " " + selectedEpisode->getLevelString());

    ui->directoryContent->clear();

    if(!selectedEpiDir->exists()) {
        ui->buttonDownload->setDisabled(false);
        ui->buttonStart->setDisabled(true);
        ui->buttonSetup->setDisabled(true);
    } else {
        ui->buttonDownload->setDisabled(true);
        ui->buttonStart->setDisabled(false);
        ui->buttonSetup->setDisabled(false);

        QFileInfoList filesList = selectedEpiDir->entryInfoList();


        foreach(QFileInfo fileInfo, filesList)
        {
            QString iconName = getFileIconName(fileInfo);
            if(iconName == "false") {
                continue;
            }

            QTreeWidgetItem* item = new QTreeWidgetItem(ui->directoryContent);
            item->setText(0,fileInfo.fileName());

            item->setText(1,QString::number(fileInfo.size()));
            item->setIcon(0,QIcon(iconName));
            item->setText(2,fileInfo.filePath());
        }
        if(selectedEpisode->getGameExe().isNull()) {
            selectedEpisode->setGameExe(searchGameExe(selectedEpiDir));
        }
    }
}

/**
  *  Helper - get the right icon for filetype
  *
  *  @param QFileInfo fileInfo
  *  @return QString
  *
**/
QString MainWindow::getFileIconName(QFileInfo fileInfo) {
    QString suffix = fileInfo.suffix().toLower();

    if(suffix == "txt" && fileInfo.fileName().toLower() != "extractoutput.txt") {
        return ":/icons/images/01.png";
    }
    if(suffix == "jpg" || suffix == "png" || suffix == "bmp" ) {
        return ":/icons/images/02.png";
    }
    if(suffix == "mp3") {
        return ":/icons/images/03.png";
    }
    if(suffix == "avi") {
        return ":/icons/images/04.png";
    }
    if(suffix == "doc") {
        return ":/icons/images/05.png";
    }
    if(suffix == "pdf") {
        return ":/icons/images/06.png";
    }
    if(suffix == "zip") {
        return ":/icons/images/07.png";
    }
    if(suffix == "tra") {
        if(fileInfo.fileName().toLower() == "english.tra") {
            return ":/icons/images/08.png";
        }
        if(fileInfo.fileName().toLower() == "french.tra" || fileInfo.fileName().toLower() == "français.tra") {
            return ":/icons/images/09.png";
        }
        if(fileInfo.fileName().toLower() == "spanish.tra") {
            return ":/icons/images/10.png";
        }
        if(fileInfo.fileName().toLower() == "italian.tra" || fileInfo.fileName().toLower() == "italiano.tra") {
            return ":/icons/images/13.png";
        }
        if(fileInfo.fileName().toLower() == "russian.tra") {
            return ":/icons/images/14.png";
        }
    }
    if(/*Info.filfileeName().toLower() == "agssave"*/suffix.toInt()) {
        return ":/icons/images/11.png";
    }
    if(suffix == "agr") {
        return ":/icons/images/12.png";
    }

    return "false";
}

void MainWindow::openAdditionalFile(QTreeWidgetItem* item) {
    QFileInfo* fileInfo = new QFileInfo(epiDir->absolutePath() + "/" + selectedEpisode->getDirectory() + "/" + item->text(0));
    if(fileInfo->exists()) {
        QString suffix = fileInfo->suffix().toLower();
        if(suffix == "tra") {
            if(QMessageBox::question(this, tr("Setup starten?"), tr("Die entsprechende Datei ist eine Übersetzung für das Spiel. Möchtest du das Spielsetup, wenn möglich, jetzt starten?"), QMessageBox::No | QMessageBox::Yes, QMessageBox::Yes) == QMessageBox::Yes) {
                setupEpisode();
            }
        } else if(/*Info.filfileeName().toLower() == "agssave"*/suffix.toInt()) {
            if(QMessageBox::question(this, tr("Spiel starten?"), tr("Die entsprechende Datei ist ein Savegame für das Spiel. Möchtest du das Spiel, wenn möglich, jetzt starten?"), QMessageBox::No | QMessageBox::Yes, QMessageBox::Yes) == QMessageBox::Yes) {
                startEpisode();
            }
        } else if(suffix == "agr") {
            QMessageBox::information(this, "Kann Datei nicht öffen", tr("Die entsprechende Datei kann jetzt nicht geöffnet werden. Versuche, die Datei mit dem Adventure Game Studio zu öffnen!"));
        } else {
            QDesktopServices::openUrl(QUrl::fromLocalFile(fileInfo->absoluteFilePath()));
        }
    }
}

void MainWindow::setImage() {
    if(selectedEpisode->getImage().isNull()) {
        setNoimage();
        return;
    }
    QString image = screenDir->absolutePath() + "/" + selectedEpisode->getImage();
    QFile file(image);
    if(file.exists()) {
        scene = new QGraphicsScene();
        scene->addPixmap(QPixmap(image));
        scene->addPixmap(selectedEpisode->getEdgarImage());
        ui->epiScreenshoot->setScene(scene);
    } else {
        setNoimage();

    }
}

/**
  *  Set default image if no image is available
  *
  *  @return void
  *
**/
void MainWindow::setNoimage() {
    scene = new QGraphicsScene();
    scene->addPixmap(QPixmap(":/images/images/undefined.png"));
    if(selectedEpisode != NULL) {
        scene->addPixmap(selectedEpisode->getEdgarImage());
    }
    ui->epiScreenshoot->setScene(scene);
}

/**
  *  Set aktionslisten fields to "N/A"
  *
  *  @return void
  *
**/
void MainWindow::unsetAktionsliste() {
    ui->actions->setText(tr("<b>Aktionen:</b> N/A"));
    ui->length->setText(tr("<b>Länge:</b> N/A"));
    ui->difficulty->setText(tr("<b>Schwierigkeit:</b> N/A"));
}

/**
  *  Reset widgets when no episode is selected
  *
  *  @return void
  *
**/
void MainWindow::noEpiSelected() {
    setNoimage();
    unsetAktionsliste();
    ui->epiDescription->setPlainText("");
    ui->buttonDownload->setDisabled(true);
    ui->buttonSetup->setDisabled(true);
    ui->buttonStart->setDisabled(true);
    ui->directoryContent->clear();

    ui->actions->setText(tr("<b>Aktionen:</b> N/A"));
    ui->length->setText(tr("<b>Länge:</b> N/A"));
    ui->difficulty->setText(tr("<b>Schwierigkeit:</b> N/A"));
    return;
}

/**
  *  Set correct widget states when a download is running
  *
  *  @return void
  *
**/
void MainWindow::setControlsDownloading()
{
    ui->buttonDownload->setDisabled(true);
    ui->buttonSetup->setDisabled(true);
    ui->buttonStart->setDisabled(true);
    ui->buttonDownloadall->setDisabled(true);
    ui->buttonUpdate->setDisabled(true);

    ui->progressBar->setDisabled(false);
    ui->labelUpdate->setText(tr("Downloading ..."));
    ui->tabWidget->setDisabled(true);
    currentTreeWidget->setDisabled(true);

}

/**
  *  Set correct widget states when all downloads are finished
  *
  *  @return void
  *
**/
void MainWindow::setControlsDownloadFinished()
{
    episodeSelected();
    ui->buttonDownloadall->setDisabled(false);
    ui->buttonUpdate->setDisabled(false);

    ui->progressBar->setDisabled(true);
    ui->progressBar->setValue(0);
    ui->progressBar->setMaximum(100);
    ui->progressBar->setMinimum(0);
    ui->labelUpdate->setText(tr("Für ein Update auf \"Update\" klicken!"));
    ui->tabWidget->setDisabled(false);
    currentTreeWidget->setDisabled(false);
}

/**
  *  Check if the json files require a update
  *
  *  @return void
  *
**/
void MainWindow::checkIfUpdateNeeded()
{
    QJsonParseError parseError;
    QJsonParseError existingParseError;
    QDateTime existingLastupdate;
    QByteArray data;
    bool updateNeeded = false;
    QJsonDocument json = QJsonDocument::fromJson(downloader->downloadedData(), &parseError);

    // Set progressbar scrolling
    ui->progressBar->setMaximum(0);
    ui->progressBar->setMinimum(0);

    QFile file;
    file.setFileName(settings->value("epiJson").toString());
    if(file.exists()) {
        file.open(QIODevice::ReadOnly);
        data = file.readAll();
        QJsonDocument existingJson = QJsonDocument::fromJson(data, &existingParseError);

        if(existingParseError.error == QJsonParseError::NoError)
        {
            QJsonObject ob = existingJson.object();
            QJsonObject meta = ob["meta"].toObject();
            existingLastupdate = QDateTime::fromString(meta["lastupdate"].toString(), "yyyy-MM-dd hh:mm:ss");
        } else {
            qDebug() << "Updating because existing file is corrupt.";
            updateNeeded = true;
        }
    } else {
        qDebug() << "Updating because file does not exist.";
        updateNeeded = true;
    }

    if(parseError.error == QJsonParseError::NoError)
    {
        QJsonObject ob = json.object();
        QJsonObject meta = ob["meta"].toObject();
        QDateTime lastupdate = QDateTime::fromString(meta["lastupdate"].toString(), "yyyy-MM-dd hh:mm:ss");
        Version lastVersion(meta["lastlauncherversion"].toString().toStdString().c_str());
        Version currentVersion(QApplication::applicationVersion().toStdString().c_str());

        if(currentVersion < lastVersion) {
            QMessageBox::information(0, tr("Update verfügbar!"),
                                     tr("Eine neue MMMLauncher Version ist verfügbar!\nInstallierte Version: %1  Aktuelle Version: %2")
                                        .arg(
                                             QString(QApplication::applicationVersion()),
                                             meta["lastlauncherversion"].toString()
                                         )
                                    );
        }

        if(meta.contains("specialmessage")) {
            QJsonObject specialmessage =  meta["specialmessage"].toObject();
            Version forVersion = Version(specialmessage["forversion"].toString().toStdString().c_str());

            bool notify = false;
            if(specialmessage["operator"].toString() == "<" && currentVersion < forVersion)
                notify = true;
            else if(specialmessage["operator"].toString() == "==" && currentVersion == forVersion)
                notify = true;
            else if(specialmessage["operator"].toString() == "<=" && currentVersion <= forVersion)
                notify = true;


            if(notify) {
                QMessageBox::information(0, tr("Wichtige Meldung!"),
                                         specialmessage["message"].toString());
            }
        }

        if(!updateNeeded) {
            if(existingLastupdate < lastupdate) {
                qDebug() << "Updating because existing file is too old.";
                initUpdate();
            } else {
                QByteArray data2;
                qDebug() << "Using existing file";
                data2 = loadCacheFile(settings->value("categoryJson").toString());
                if(!data2.isEmpty()) {
                    updateCategories(data2);
                } else {
                    updatingAny = true;
                    downloadCategories();
                }
                data2 = loadCacheFile(settings->value("authorJson").toString());
                if(!data2.isEmpty()) {
                    updateAuthors(data2);
                } else {
                    updatingAny = true;
                    downloadAuthors();
                }

                if(!updatingAny) {
                    updateGames(data);
                }

            }
        }
    } else {
        qDebug() << "Lastupdate api broken ... uhm.";
        initUpdate();
    }
    if(updateNeeded) {
        initUpdate();
    }
    file.close();

}

/**
  *  Download all json files
  *
  *  @return void
  *
**/
void MainWindow::initUpdate()
{
    updatingAny = true;
    downloadCategories();
    downloadAuthors();
    downloadGames();
    // TODO Call updateGames(data);
}

/**
  *  Download the category json file
  *
  *  @return void
  *
**/
void MainWindow::downloadCategories() {
    categoriesUpdate = true;
    qDebug() << "downloading categories";
    categoryDownloader = new FileDownloader(QUrl(settings->value("apiUrl").toString() + "/category"), this);
    connect(categoryDownloader, SIGNAL(downloaded()), SLOT(updateCategories()));
}

/**
  *  Download the author json file
  *
  *  @return void
  *
**/
void MainWindow::downloadAuthors() {
    authorsUpdate = true;
    qDebug() << "downloading authors";
    authorDownloader = new FileDownloader(QUrl(settings->value("apiUrl").toString() + "/author"), this);
    connect(authorDownloader, SIGNAL(downloaded()), SLOT(updateAuthors()));
}

/**
  *  Download the episode json file
  *
  *  @return void
  *
**/
void MainWindow::downloadGames() {
    episUpdate = true;
    qDebug() << "downloading games";
    downloader = new FileDownloader(QUrl(settings->value("apiUrl").toString() + "/game"), this);
    connect(downloader, SIGNAL(downloaded()), SLOT(updateGames()));
}

/**
  *  Cache and update categories
  *apiUrl
  *  @return void
  *
**/
void MainWindow::updateCategories() {
    // Cache and parse categories
    cacheFile(settings->value("categoryJson").toString(), categoryDownloader->downloadedData());
    updateCategories(categoryDownloader->downloadedData());
    // Update authors

}

/**
  *  Update categories
  *
  *  @param QByteArray data
  *  @return void
  *
**/
void MainWindow::updateCategories(QByteArray data) {
    categories = new CategoryParser(data);
    categories->parse();

    foreach (Category* category, categories->categories) {
        QTreeWidgetItem* cat;

        if(category->getParentUid() != -1) {
            if(!treeCategories.contains(category->getParentUid()))
                continue;
            cat = new QTreeWidgetItem(treeCategories.find(category->getParentUid()).value());
        } else {
            cat = new QTreeWidgetItem(ui->treeWidget);
            QFont font = cat->font(0);
            font.setBold(true);
            cat->setFont(0, font);
        }

        cat->setText(0, category->getTitle());
        cat->setFlags(Qt::ItemIsEnabled);
        treeCategories.insert(category->getUid(), cat);
    }

    ui->treeWidget->expandAll();

    if(updatingAny && (authors->parsed || !authorsUpdate) && !episUpdate) {
        updateGames(loadCacheFile(settings->value("epiJson").toString()));
    }

    qDebug() << "Categories parsed";
}

/**
  *  Cache and update authors
  *
  *  @return void
  *
**/
void MainWindow::updateAuthors() {
    // Cache and parse categories
    cacheFile(settings->value("authorJson").toString(), authorDownloader->downloadedData());
    updateAuthors(authorDownloader->downloadedData());
}

/**
  *  Update authors
  *
  *  @param QByteArray data
  *  @return void
  *
**/
void MainWindow::updateAuthors(QByteArray data) {
    authors = new AuthorParser(data);
    authors->parse();

    if(updatingAny && (categories->parsed || !categoriesUpdate) && !episUpdate) {
        updateGames(loadCacheFile(settings->value("epiJson").toString()));
    }
    qDebug() << "Authors parsed";
}

/**
  *  Cache and update episoded if categories and authors are already updated
  *
  *  @return void
  *
**/
void MainWindow::updateGames() {
    // Cache and parse games
    cacheFile(settings->value("epiJson").toString(), downloader->downloadedData());
    episUpdate = false;
    if((!authorsUpdate && !categoriesUpdate) || (categories->parsed && authors->parsed)) {
            updateGames(loadCacheFile(settings->value("epiJson").toString()));
    }
    qDebug() << "Games downloaded";
}

/**
  *  Update episodes
  *
  *  @param QByteArray data
  *  @return void
  *
**/
void MainWindow::updateGames(QByteArray data)
{
    qDebug() << "updateGames";
    epis = new EpiParser(data);
    epis->parse();

    foreach (Episode* episode, epis->episodes) {
        addGameWidgetItem(episode);
    }

    ui->progressBar->setMaximum(0);
    ui->progressBar->setMinimum(100);
}

/**
  *  Add a game entry to the main treewidget
  *
  *  @param Episode* episode
  *  @return void
  *
**/
void MainWindow::addGameWidgetItem(Episode* episode)
{
    if(!treeCategories.contains(episode->getCategoryUid()))
        return;
    QTreeWidgetItem *item = new QTreeWidgetItem(treeCategories.find(episode->getCategoryUid()).value());
    addGameWidgetItem(episode, item);
}

/**
  *  Add a game entry to the given treewidget
  *
  *  @param Episode* episode
  *  @param QTreeWidget* widget
  *  @return void
  *
**/
void MainWindow::addGameWidgetItem(Episode* episode, QTreeWidget* widget)
{
    QTreeWidgetItem *item = new QTreeWidgetItem(widget);
    addGameWidgetItem(episode, item);
}

/**
  *  Add a game entry to the given treewidgetitem
  *
  *  @param Episode* episode
  *  @param QTreeWidgetItem* item
  *  @return void
  *
**/
void MainWindow::addGameWidgetItem(Episode* episode, QTreeWidgetItem* item)
{
    QString author;
    Category* category = categories->categories.find(episode->getCategoryUid()).value();

    if(!authors->authors.contains(episode->getAuthorUid())) {
        author = tr("Unknown");
    } else {
        author = authors->authors.find(episode->getAuthorUid()).value()->getName();
    }

    item->setIcon(0, category->getIcon());
    item->setText(0, episode->getTitle());
    item->setText(1, author);
    item->setData(0,Qt::UserRole, episode->getUid()); // unique ID;
}

/**
  *  Cache a file
  *
  *  @param QString filename
  *  @param QByteArray data
  *  @return void
  *
**/
void MainWindow::cacheFile(QString filename, QByteArray data)
{
    QFile file;
    file.setFileName(filename);
    file.remove();
    file.open(QIODevice::WriteOnly);
    file.write(data);
    file.close();
}

/**
  *  Load a cached file
  *
  *  @param QString filename
  *  @return QByteArray
  *
**/
QByteArray MainWindow::loadCacheFile(QString filename)
{
    QFile file;
    file.setFileName(filename);
    if(file.exists()) {
        file.open(QIODevice::ReadOnly);
        return file.readAll();
    }
    return QByteArray();
}

/**
  *  Slot - Download the selected episode
  *
  *  @return void
  *
**/
void MainWindow::downloadEpisode()
{
    downloadEpisode(selectedEpisode);
}

/**
  *  Slot - Download all episodes
  *
  *  @return void
  *
**/
void MainWindow::downloadAll()
{
    if(QMessageBox::question(this, tr("Alles laden?"), tr("Bist du sich sicher, dass du alle Episoden laden möchten?"), QMessageBox::No | QMessageBox::Yes, QMessageBox::Yes) == QMessageBox::Yes) {
        downloadAllIndex = 0;
        downloadingAll = true;
        downloadNext();
    }
}

/**
  *  Download the given episode
  *
  *  @param Episode* episode
  *  @return void
  *
**/
void MainWindow::downloadEpisode(Episode* episode)
{
    qDebug() << QString("Downloading");
    setControlsDownloading();

    epidownloader = new EpiDownloader(episode->getDownloadUrl(), this);

    connect(epidownloader, SIGNAL(downloaded()), SLOT(finishedDownload()));
    connect(epidownloader, SIGNAL(readReady(QByteArray)), SLOT(readReady(QByteArray)));
    connect(epidownloader, SIGNAL(downloadProgress(qint64, qint64, double, QString)), SLOT(downloadProgress(qint64, qint64/*, double, QString*/)));

    downloadFile = new QFile(settings->value("baseDir").toString() + "/" + "temp");
    downloadFile->open(QIODevice::WriteOnly);

    if (!screenDir->exists()) {
        screenDir->mkpath(".");
    }
    if(!episode->getImage().isNull()) {
        QUrl screenUrl = QUrl(episode->getScreenshotUrl());
        screenDownloader = new FileDownloader(screenUrl, this);
        connect(screenDownloader, SIGNAL(downloaded()), SLOT(downloadedScreenshot()));

        screenDownloadFile = new QFile(screenDir->absolutePath() + "/" + episode->getImage());
        screenDownloadFile->open(QIODevice::WriteOnly);
        qDebug() << screenDir->absolutePath() + "/" + episode->getImage();
    }
}

void MainWindow::downloadedScreenshot()
{
    screenDownloadFile->write(screenDownloader->downloadedData());
    screenDownloadFile->close();
    setImage();
}

/**
  *  Download the next episode
  *
  *  @return void
  *
**/
void MainWindow::downloadNext()
{
    qDebug() << epis->episodes.count() << " - " << downloadAllIndex;
    if(downloadAllIndex <= epis->episodes.count()) {
        if(epis->episodes.contains(downloadAllIndex)) {
            Episode * episode = epis->episodes[downloadAllIndex];
            selectedEpisode = episode;
            downloadEpisode(episode);
        } else {
            downloadAllIndex++;
            downloadNext();
            return;
        }
        downloadAllIndex++;
    } else {
        downloadingAll = false;
        downloadAllIndex = 0;
        setControlsDownloadFinished();
    }
}

/**
  *  Slot - Fired every there and then from the downloader - Set the downloadprogress to progressbar
  *
  *  @param qint64 bytesReceived
  *  @param qint64 bytesTotal
  *  @return void
  *
**/
void MainWindow::downloadProgress(qint64 bytesReceived, qint64 bytesTotal/*, double speed, QString unit*/)
{
    double progress = ((double) bytesReceived / (double) bytesTotal) * 100;
    ui->progressBar->setValue(progress);
}

/**
  *  Slot - Fired when bytes from downloader are ready - Write ready bytes to file
  *
  *  @param QByteArray bytes
  *  @return void
  *
**/
void MainWindow::readReady(QByteArray bytes)
{
    downloadFile->write(bytes);
}

/**
  *  Slot - Fired when a download is finished
  *
  *  @return void
  *
**/
void MainWindow::finishedDownload()
{
    downloadFileInfo = new QFileInfo(epidownloader->filename);
    downloadFileName = settings->value("baseDir").toString() + "/" + downloadFileInfo->fileName();

    downloadFile->rename(downloadFileName);
    qDebug() << QString("Downloaded " + downloadFileInfo->fileName());

    if (!epiDir->exists()) {
        epiDir->mkpath(".");
    }

    epiDir->mkpath(selectedEpisode->getDirectory());

    int atype;

    if(downloadFileInfo->suffix() == "zip") {
        atype = ArchiveType::ZipArchive;
    } else if(downloadFileInfo->suffix() == "rar") {
        atype = ArchiveType::RarArchive;
    } else if(downloadFileInfo->suffix() == "7z") {
        atype = ArchiveType::SevenZipArchive;
    } else if(downloadFileInfo->suffix() == "exe") {
        downloadFile->rename(selectedEpisode->getDirectory() + "/" + downloadFileInfo->fileName());
        if(downloadingAll) {
            downloadNext();
            return;
        }

        setControlsDownloadFinished();
        return;
    } else {
        downloadFile->remove();
        QMessageBox::critical(0, "Error", "Keine passende Methode gefunden, um das Archiv zu entpacken!");
        setControlsDownloadFinished();
        return;
    }

    QDir *downloadedEpiDir = new QDir(epiDir->absolutePath() + "/" + selectedEpisode->getDirectory());

    QExtract *extract = new QExtract(downloadedEpiDir->absolutePath(), downloadFileName, atype);
    extract->start();
    connect(extract, SIGNAL(endExtractSignal()), SLOT(extractDone()));

    ui->progressBar->setMaximum(0);
    ui->progressBar->setMinimum(0);
}

/**
  *  Slot - Fired when extracting finished
  *
  *  @return void
  *
**/
void MainWindow::extractDone()
{
    qDebug() << "Extract done";
    QDir *downloadedEpiDir = new QDir(epiDir->absolutePath() + "/" + selectedEpisode->getDirectory());
    cleanUpDirectory(downloadedEpiDir);
    downloadFile->remove();
    downloadFile->close();
    if(downloadingAll) {
        downloadNext();
        return;
    }
    setControlsDownloadFinished();
}


/**
  *  Clean up directory -> remove unneeded directorys
  *
  *  @param QDir *dir
  *  @return void
  *
**/
void MainWindow::cleanUpDirectory(QDir *dir) {

    if(isUnneededDirectory(dir)) {
        QString firstDir = getFirstDirectory(dir);
        if(firstDir != "false") {
            moveUp(firstDir, dir);
            cleanUpDirectory(dir);
        }
    }
}

/**
  *  Check if a directory is unneeded
  *
  *  @param QDir *dir
  *  @return bool
  *
**/
bool MainWindow::isUnneededDirectory(QDir *dir) {
    QFileInfoList filesList = dir->entryInfoList();
    int dirCount = 0;
    int exeCount = 0;
    foreach(QFileInfo fileInfo, filesList)
    {
        if(fileInfo.isDir()) {
            dirCount++;
        } else if(fileInfo.suffix() == "exe") {
            exeCount++;
        }
    }
    return dirCount > 0 && exeCount < 1;
}

/**
  *  Get the first directory in a directory
  *
  *  @param QDir *dir
  *  @return QString
  *
**/
QString MainWindow::getFirstDirectory(QDir *dir) {
    QFileInfoList filesList = dir->entryInfoList(QDir::AllEntries | QDir::NoDotAndDotDot);
    foreach(QFileInfo fileInfo, filesList)
    {
        if(fileInfo.isDir()) {
            return fileInfo.absoluteFilePath();
        }
    }
    return "false";
}

/**
  *  Move contents of directory one path up and remove the dir afterwards
  *
  *  @param QDir dir
  *  @param QDir *destDir
  *  @return void
  *
**/
void MainWindow::moveUp(QDir dir, QDir *destDir) {

    QFileInfoList filesList = dir.entryInfoList(QDir::AllEntries | QDir::NoDotAndDotDot);
    foreach(QFileInfo fileInfo, filesList)
    {
        QFile().rename(fileInfo.absoluteFilePath(), destDir->absoluteFilePath(fileInfo.fileName()));
    }

    dir.rmdir(dir.absolutePath());
}

/**
  *  Search the game exe in the given directory
  *
  *  @param QDir *dir
  *  @return QString
  *
**/
QString MainWindow::searchGameExe(QDir *dir) {
    QFileInfoList filesList = dir->entryInfoList();
    foreach(QFileInfo fileInfo, filesList)
    {
        if(fileInfo.suffix() == "exe" && fileInfo.fileName().toLower() != "winsetup") {
            return fileInfo.absoluteFilePath();
        }
    }
    return "false";
}

/**
  *  Start the selected episode
  *
  *  @return void
  *
**/
void MainWindow::startEpisode() {
    QStringList parameters;
    QProcess *process = new QProcess();
    process->setWorkingDirectory(epiDir->absolutePath() + "/" + selectedEpisode->getDirectory());
    #ifdef Q_OS_LINUX
        parameters << "-windowed";
        process->start("ags", parameters);
    #elif defined(Q_OS_WIN)
        process->start(selectedEpisode->getGameExe(), parameters);
    #endif
}

/**
  *  Start the setup for the selected episode
  *
  *  @return void
  *
**/
void MainWindow::setupEpisode() {
    QStringList parameters;
    QProcess *process = new QProcess();
    QString epiPath = QString(epiDir->absolutePath() + "/" + selectedEpisode->getDirectory() + "/");
    process->setWorkingDirectory(epiPath);
    #ifdef Q_OS_LINUX
      QMessageBox::information(0, "Information", tr("Diese Funktion steht unter Linux derzeit nicht zur verfügung."));
    #elif defined(Q_OS_WIN)
      process->start(epiPath + "winsetup.exe", parameters);
      qDebug() << process->errorString();
    #endif
}



/**
 *
 * Tabs
 *
 */

/**
  * Slot - fired when the new-tab-button is clicked - show the new tab dialog
  *
  *  @return void
  *
**/
void MainWindow::newTabButton()
{
    m_newTabDialog = new NewTabDialog(authors, categories);
    connect(m_newTabDialog, SIGNAL(accepted()), this, SLOT(openNewTab()));
    m_newTabDialog->show();
}

/**
  * Slot - fired when the new-tab-dialog is closed successfully - add a new tab and a treewidget in it
  *
  *  @return void
  *
**/
void MainWindow::openNewTab()
{
    QStringList headerLabels;
    headerLabels<< tr("Title") << tr("Author");

    QTreeWidget* tabContent = new QTreeWidget();
    tabContent->setHeaderLabels(headerLabels);
    tabContent->header()->setStretchLastSection(false);
    tabContent->setRootIsDecorated(false);
    tabContent->setItemsExpandable(true);
    tabContent->setIndentation(15);
    tabContent->setExpandsOnDoubleClick(false);
    tabContent->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tabContent->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    setupTreewidget(tabContent);

    QTreeWidgetItem* cat;
    cat = new QTreeWidgetItem(tabContent);
    QFont font = cat->font(0);
    font.setBold(true);
    cat->setFont(0, font);
    cat->setFlags(Qt::ItemIsEnabled);


    int newtab = ui->tabWidget->addTab(tabContent, "");

    connect(tabContent, SIGNAL(itemSelectionChanged()), SLOT(episodeSelected()));

    bool catdone = false;
    foreach (Episode* episode, epis->episodes) {
        if(m_newTabDialog->getTabType() == NewTabDialog::CatgeoryTab) {
            Category* epiCat = categories->categories.find(episode->getCategoryUid()).value();
            if(episode->getCategoryUid() != m_newTabDialog->getCategory() && epiCat->getParentUid() != m_newTabDialog->getCategory())
                continue;
            if(!catdone) {
                catdone = true;
                cat->setText(0, epiCat->getTitle());
                ui->tabWidget->setTabText(newtab, epiCat->getTitle());
            }
        } else if(m_newTabDialog->getTabType() == NewTabDialog::AuthorTab) {
            Author* author = authors->authors.find(m_newTabDialog->getAuthor()).value();
            if(episode->getAuthorUid() != m_newTabDialog->getAuthor())
                continue;
            if(!catdone) {
                catdone = true;
                cat->setText(0, author->getName());
                ui->tabWidget->setTabText(newtab, author->getName());
            }
        } else {
            return;
        }
        addGameWidgetItem(episode, new QTreeWidgetItem(cat));
    }
    tabContent->expandAll();
    ui->tabWidget->setCurrentIndex(newtab);


}

/**
  * Slot - fired when a tab should be closed - remove tab and treewidget
  *
  *  @return void
  *
**/
void MainWindow::closeTab(int index)
{
    if (index == -1) {
            return;
        }

        QWidget* tabItem = ui->tabWidget->widget(index);
        // Removes the tab at position index from this stack of widgets.
        // The page widget itself is not deleted.
        ui->tabWidget->removeTab(index);

        delete(tabItem);
        tabItem = nullptr;
}

/**
 *
 * About dialog
 *
 */

/**
  * Slot - fired when the about-button is clicked - show the about dialog
  *
  *  @return void
  *
**/
void MainWindow::aboutMMMLauncher()
{
    if (!m_aboutDialog) {
        m_aboutDialog = new AboutDialog(this);
        connect(m_aboutDialog, SIGNAL(finished(int)), this, SLOT(destroyAboutDialog()));
    }
    m_aboutDialog->show();
}

/**
  * Slot - fired when the about dialog should be closed - close about dialog
  *
  *  @return void
  *
**/
void MainWindow::destroyAboutDialog()
{
    if (m_aboutDialog) {
        m_aboutDialog->deleteLater();
        m_aboutDialog = 0;
    }
}


/**
 *
 * Settings dialog
 *
 */

/**
  * Slot - fired when the settings-button is clicked - show the settings dialog
  *
  *  @return void
  *
**/
void MainWindow::settingsDialog()
{
    m_settingsDialog = new SettingsDialog(settings->value("baseDir").toString(), settings->value("aktionsliste").toBool(), this);
    connect(m_settingsDialog, SIGNAL(finished(int)), this, SLOT(destroySettingsDialog()));
    m_settingsDialog->show();
}

/**
  * Slot - fired when the settings dialog should be closed - close settings dialog
  *
  *  @return void
  *
**/
void MainWindow::destroySettingsDialog()
{
    if (m_settingsDialog) {
        settings->setValue("baseDir", m_settingsDialog->getFolder());
        settings->setValue("aktionsliste", m_settingsDialog->getAktionslisteEnabled());
        if(!m_settingsDialog->getAktionslisteEnabled()) {
            unsetAktionsliste();

            ui->actions->hide();
            ui->length->hide();
            ui->difficulty->hide();
        } else {
            ui->actions->show();
            ui->length->show();
            ui->difficulty->show();
        }
        updateSettings();
        m_settingsDialog->deleteLater();
        m_settingsDialog = 0;
    }
}

// we create the menu entries dynamically, dependent on the existing translations.
void MainWindow::createLanguageMenu(void)
{
 QActionGroup* langGroup = new QActionGroup(ui->menuLanguage);
 langGroup->setExclusive(true);

 connect(langGroup, SIGNAL (triggered(QAction *)), this, SLOT (slotLanguageChanged(QAction *)));

 // format systems language
 QString defaultLocale = QLocale::system().name(); // e.g. "de_DE"
 defaultLocale.truncate(defaultLocale.lastIndexOf('_')); // e.g. "de"

 QDir dir(":/languages/");
 QStringList fileNames = dir.entryList(QStringList("mmml_*.qm"));

 for (int i = 0; i < fileNames.size(); ++i) {
  // get locale extracted by filename
  QString locale;
  locale = fileNames[i];
  locale.truncate(locale.lastIndexOf('.'));
  locale.remove(0, locale.indexOf('_') + 1);

 QString lang = QLocale::languageToString(QLocale(locale).language());
 QIcon ico(QString(":/images/images/"+locale+".png"));

 QAction *action = new QAction(ico, lang, this);
 action->setCheckable(true);
 action->setData(locale);

 ui->menuLanguage->addAction(action);
 langGroup->addAction(action);

 // set default translators and language checked
 if (defaultLocale == locale)
 {
 action->setChecked(true);
 }
 }
}

// Called every time, when a menu entry of the language menu is called
void MainWindow::slotLanguageChanged(QAction* action)
{
 if(0 != action) {
  // load the language dependant on the action content
  loadLanguage(action->data().toString());
  //setWindowIcon(action->icon());
 }
}

void switchTranslator(QTranslator& translator, const QString& language)
{
 // remove the old translator
 qApp->removeTranslator(&translator);

 // load the new translator
 if(translator.load(QString(":/languages/mmml_%1.qm").arg(language)))
  qApp->installTranslator(&translator);
}

void MainWindow::loadLanguage(const QString& rLanguage)
{
 if(m_currLang != rLanguage) {
  m_currLang = rLanguage;
  QLocale locale = QLocale(m_currLang);
  QLocale::setDefault(locale);
  QString languageName = QLocale::languageToString(locale.language());
  switchTranslator(m_translator, rLanguage);
  //switchTranslator(m_translatorQt, QString("qt_%1.qm").arg(rLanguage));
 }
}
void MainWindow::changeEvent(QEvent* event)
{
 if(0 != event) {
  switch(event->type()) {
   // this event is send if a translator is loaded
   case QEvent::LanguageChange:
    ui->retranslateUi(this);
    break;

   // this event is send, if the system, language changes
   case QEvent::LocaleChange:
   {
    QString locale = QLocale::system().name();
    locale.truncate(locale.lastIndexOf('_'));
    loadLanguage(locale);
    break;
   }
   default: {
    break;
   }
  }
 }
 QMainWindow::changeEvent(event);
}
/**
 *
 * Restart / Close
 *
 */

/**
  * Slot - fired when the update button is clicked - a dirty workaround because the update runs at start
  *
  *  @return void
  *
**/
void MainWindow::restartApp()
{
    QProcess::startDetached(QApplication::applicationFilePath());
    exit(12);
}


MainWindow::~MainWindow()
{
    delete ui;
}
