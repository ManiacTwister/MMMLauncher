#ifndef EPISODE_H
#define EPISODE_H

#include <QString>
#include <QIcon>
#include "category.h"

class Episode
{
public:
    Episode();
    Episode(int id, QString title, QString description, int author, int categoryUid, QString image, QString downloadurl);
    void setCategoryUid(int newcategory);
    void setAuthorUid(int newauthor);
    void setTitle(QString newtitle);
    void setDescription(QString newdescription);
    void setDownloadUrl(QString newdownloadurl);
    void setScreenshotUrl(QString newscreenshoturl);
    void setGameExe(QString exe);
    void setEdgarCount(int newCount);
    void setVotes(int newVotes);
    void setUid(int newId);
    int getCategoryUid();
    int getAuthorUid();
    QString getTitle();
    QString getDescription();
    QIcon getCategoryIcon();
    QString getImage();
    QString getDownloadUrl();
    QString getScreenshotUrl();
    QString getGameExe();
    int getEdgarCount();
    int getVotes();
    int getUid();
private:
    int categoryUid;
    int author;
    QString title;
    QString description;
    QString downloadurl;
    QString screenshoturl;
    QString gameExe;
    int edgars;
    double votes;
    int uid;
};

#endif // EPISODE_H
