#include "episode.h"
#include <QString>
#include <QGraphicsScene>
#include "category.h"
#include <QDebug>
#include <QFileInfo>
#include <QUrl>
#include <QLabel>
#include <QPainter>
#include <QPixmap>
#include <algorithm>

Episode::Episode()
{
}

Episode::Episode(int uid, QString title, QString description, int author, int categoryUid, QString screenshotUrl, QString downloadurl, int actions, int length, int level, QStringList languages, int edgar)
{
    setTitle(title);
    setDescription(description);
    setAuthorUid(author);
    setCategoryUid(categoryUid);
    setScreenshotUrl(screenshotUrl);
    setDownloadUrl(downloadurl);
    setUid(uid);
    setActions(actions);
    setLength(length);
    setLevel(level);
    setLanguages(languages);
    setEdgar(edgar);
    gameExe = QString();
}

void Episode::setTitle(QString newtitle)
{
    title = newtitle;
}

void Episode::setDescription(QString newdesc)
{
    description = newdesc;
}

void Episode::setAuthorUid(int newauthor)
{
    author = newauthor;
}

void Episode::setCategoryUid(int newcategory)
{
    categoryUid = newcategory;
}

void Episode::setScreenshotUrl(QString newscreenshotUrl)
{
    screenshoturl = newscreenshotUrl;
}

void Episode::setDownloadUrl(QString newdownloadurl)
{
    downloadurl = newdownloadurl;
}

void Episode::setGameExe(QString exe)
{
    gameExe = exe;
}

void Episode::setUid(int newUid)
{
    uid = newUid;
}

void Episode::setActions(int newActions)
{
    actions = newActions;
}

void Episode::setLength(int newLength)
{
    length = newLength;
}

void Episode::setLevel(int newLevel)
{
    level = newLevel;
}

void Episode::setLanguages(QStringList newLanguages)
{
    languages = newLanguages;
}

void Episode::setEdgar(int newEdgar)
{
    edgar = newEdgar;
}

QString Episode::getTitle() { return title; }
QString Episode::getDescription() { return description; }
int Episode::getAuthorUid() { return author; }
int Episode::getCategoryUid() { return categoryUid; }
QIcon Episode::getCategoryIcon()
{
    switch(getCategoryUid())
    {
        case 0:
            return QIcon(":/images/images/chuck.png");
        case 1:
            return QIcon(":/images/images/redchuck.png");
        case 2:
            return QIcon(":/images/images/bluechuck.png");
        case 3:
            return QIcon(":/images/images/tr.ico");
    }
    return QIcon(":/images/images/chuck.png");
}
QString Episode::getImage() {
    QUrl screenUrl = QUrl(getScreenshotUrl());
    QFileInfo fileInf(screenUrl.path());
    return fileInf.fileName();
}
QString Episode::getDownloadUrl() { return downloadurl; }
QString Episode::getScreenshotUrl() { return screenshoturl; }
QString Episode::getGameExe() { return gameExe; }
QString Episode::getDirectory() { return title.replace(":", " -"); }
QStringList Episode::getLanguages(){ return languages; }
QLabel *Episode::getLanguageLabel() {
    QPixmap comboPixmap(std::max(getLanguages().length() * 18, 1), 11);
    comboPixmap.fill(Qt::transparent);

    QPainter painter(&comboPixmap);


    int i = 0;
    foreach(QString language, getLanguages()) {
        if(!QFile::exists(":/images/images/"+language+".png")) continue;
        QPixmap tmpImage(":/images/images/"+language+".png");
        painter.drawPixmap(i*18, 0, 16, 11, tmpImage);
        i++;

    }
    QLabel *iconlang = new QLabel("");
    iconlang->setPixmap(comboPixmap);

    return iconlang;
}

int Episode::getEdgarCount() { return edgar; }
int Episode::getVotes() { return votes; }
int Episode::getUid() { return uid; }
int Episode::getActions() { return actions; }
QString Episode::getActionsString() {
    if(actions < 1) {
        return QObject::tr("N/A");
    }
    return QString::number(actions);
}

QString Episode::getLengthString() {
    switch(length)
    {
        case 0:
            return QObject::tr("sehr kurz");
        case 1:
            return QObject::tr("kurz");
        case 2:
            return QObject::tr("mittel");
        case 3:
            return QObject::tr("lang");
        case 4:
            return QObject::tr("sehr lang");
        case 5:
            return QObject::tr("überlang");
    }
    return QObject::tr("N/A");
}
QString Episode::getLevelString() {
    switch(level)
    {
        case 0:
            return QObject::tr("sehr leicht");
        case 1:
            return QObject::tr("leicht");
        case 2:
            return QObject::tr("mittel");
        case 3:
            return QObject::tr("schwer");
        case 4:
            return QObject::tr("sehr schwer");
    }
    return QObject::tr("N/A");
}
int Episode::getEdgar()
{
    return edgar;
}
QPixmap Episode::getEdgarImage()
{
    switch(edgar)
    {
        case 0:
            return QPixmap(":/images/images/edgar_bronze.png");
        case 1:
            return QPixmap(":/images/images/edgar_silber.png");
        case 2:
            return QPixmap(":/images/images/edgar_gold.png");

    }
    return QPixmap(1,1);
}
