#ifndef EPISODE_H
#define EPISODE_H

#include <QString>
#include <QIcon>
#include <QLabel>
#include "category.h"

class Episode
{
public:
    Episode();
    Episode(int id, QString title, QString description, int author, int categoryUid, QString image, QString downloadurl, int actions, int length, int level, QStringList languages, int edgar);
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
    void setActions(int actions);
    void setLength(int length);
    void setLevel(int level);
    int getCategoryUid();
    int getAuthorUid();
    QString getTitle();
    QString getDescription();
    QIcon getCategoryIcon();
    QString getImage();
    QString getDownloadUrl();
    QString getScreenshotUrl();
    QString getGameExe();
    int getActions();
    QString getActionsString();
    QString getLengthString();
    QString getLevelString();
    int getEdgarCount();
    int getVotes();
    int getUid();
    QString getDirectory();
    QStringList getLanguages();
    QLabel *getLanguageLabel();
    void setLanguages(QStringList value);

    int getEdgar();
    void setEdgar(int edgar);
    QPixmap getEdgarImage();

private:
    int categoryUid;
    int author;
    QString title;
    QString description;
    QString downloadurl;
    QString screenshoturl;
    QString gameExe;
    int edgar;
    double votes;
    int uid;
    int actions;
    int length;
    int level;
    QStringList languages;
};

#endif // EPISODE_H
