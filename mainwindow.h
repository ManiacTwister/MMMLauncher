#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QModelIndex>
#include <QFile>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QNetworkConfigurationManager>
#include <QTreeWidgetItem>
#include <QFileInfo>
#include <QSettings>
#include <QMapIterator>
#include <QTranslator>
#include "filedownloader.h"
#include "epidownloader.h"
#include "aboutdialog.h"
#include "settingsdialog.h"
#include "Parsers/epiparser.h"
#include "Parsers/categoryparser.h"
#include "Parsers/authorparser.h"
#include "newtabdialog.h"
#include "Models/episode.h"

namespace Ui {
class MainWindow;
}


class MainWindow : public QMainWindow
{
    Q_OBJECT
protected:
 // this event is called, when a new translator is loaded or the system language is changed
 void changeEvent(QEvent*);

protected slots:
 // this slot is called by the language menu actions
 void slotLanguageChanged(QAction* action);

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    bool init();

    
private:
    AboutDialog *m_aboutDialog;
    SettingsDialog *m_settingsDialog;
    NewTabDialog *m_newTabDialog;
    QGraphicsScene *scene;
    Ui::MainWindow *ui;

    QTranslator m_translator; // contains the translations for this application
    QTranslator m_translatorQt; // contains the translations for qt
    QString m_currLang; // contains the currently loaded language
    QString m_langPath; // Path of language files. This is always fixed to /languages.

    FileDownloader *downloader;
    FileDownloader *categoryDownloader;
    FileDownloader *authorDownloader;
    FileDownloader *screenDownloader;
    EpiDownloader *epidownloader;

    QTreeWidget *currentTreeWidget;
    EpiParser *epis = new EpiParser();
    Episode *selectedEpisode = NULL;

    CategoryParser *categories = new CategoryParser();
    AuthorParser *authors = new AuthorParser();

    QFile *downloadFile;
    QFileInfo *downloadFileInfo;
    QString downloadFileName;
    QNetworkReply *downloadReply;

    QFile *screenDownloadFile;

    /*QTreeWidgetItem *catEpisodes;
    QTreeWidgetItem *catSpecials;
    QTreeWidgetItem *catTrailerDemo;*/
    QTreeWidgetItem *catTrash;
    QMap <int, QTreeWidgetItem*> treeCategories;

    QSettings *settings;
    QDir *epiDir;
    QDir *screenDir;

    bool updatingAny = false;
    bool authorsUpdate = false;
    bool categoriesUpdate = false;
    bool episUpdate = false;

    bool downloadingAll = false;
    int downloadAllIndex = 0;
    //QMapIterator<int, Episode*> downloadAllIterator();

    void readingReadyBytes();
    void setNoimage();
    void unsetAktionsliste();
    void setImage();
    void noEpiSelected();
    void setControlsDownloading();
    void setControlsDownloadFinished();
    QString getFileIconName(QFileInfo fileInfo);
    void cleanUpDirectory(QDir *dir);
    bool isUnneededDirectory(QDir *dir);
    QString getFirstDirectory(QDir *dir);
    void moveUp(QDir dir, QDir *destDir);
    QString searchGameExe(QDir *dir);
    void initUpdate();
    void updateGames(QByteArray data);
    void updateCategories(QByteArray data);
    void updateAuthors(QByteArray data);
    void cacheFile(QString filename, QByteArray data);
    QByteArray loadCacheFile(QString filename);
    void downloadCategories();
    void downloadAuthors();
    void downloadGames();
    void addGameWidgetItem(Episode* episode);
    void addGameWidgetItem(Episode* episode, QTreeWidgetItem* parent);
    void addGameWidgetItem(Episode* episode, QTreeWidget* widget);
    void setupTreewidget(QTreeWidget* widget);
    void setCurrentTreeWidget();
    void downloadEpisode(Episode* episode);
    void downloadNext();
    void updateSettings();
    QNetworkConfigurationManager networkManager;

    // loads a language by the given language shortcur (e.g. de, en)
    void loadLanguage(const QString& rLanguage);

    // creates the language menu dynamically from the content of m_langPath
    void createLanguageMenu(void);


private slots:
    void updateGames();
    void updateCategories();
    void updateAuthors();
    void restartApp();
    void checkIfUpdateNeeded();
    void aboutMMMLauncher();
    void destroyAboutDialog();
    void settingsDialog();
    void destroySettingsDialog();
    void episodeSelected();
    /*void episodeSelected(QModelIndex row);*/
    void downloadEpisode();
    void downloadAll();
    void finishedDownload();
    void readReady(QByteArray bytes);
    void downloadProgress(qint64 bytesReceived, qint64 bytesToal/*, double speed, QString unit*/);
    void extractDone();
    void startEpisode();
    void setupEpisode();
    void newTabButton();
    void openNewTab();
    void onTabChanged();
    void closeTab(int index);
    void openAdditionalFile(QTreeWidgetItem* item);
    void downloadedScreenshot();
};

#endif // MAINWINDOW_H
