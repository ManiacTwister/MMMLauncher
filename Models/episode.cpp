#include "episode.h"
#include <QString>
#include <QGraphicsScene>
#include "category.h"
#include <QDebug>
#include <QFileInfo>
#include <QUrl>

Episode::Episode()
{
}

Episode::Episode(int uid, QString title, QString description, int author, int categoryUid, QString screenshotUrl, QString downloadurl)
{
    setTitle(title);
    setDescription(description);
    setAuthorUid(author);
    setCategoryUid(categoryUid);
    setScreenshotUrl(screenshotUrl);
    setDownloadUrl(downloadurl);
    setUid(uid);
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
int Episode::getEdgarCount() { return edgars; }
int Episode::getVotes() { return votes; }
int Episode::getUid() { return uid; }
