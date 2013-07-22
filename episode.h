#ifndef EPISODE_H
#define EPISODE_H

#include <QString>
#include <QIcon>
#include "category.h"

class Episode
{
public:
    Episode();
    Episode(QString title, QString description, QString author, int category, QString image);
    void setCategory(int newcategory);
    void setAuthor(QString newauthor);
    void setTitle(QString newtitle);
    void setDescription(QString newdescription);
    void setImage(QString image);
    int getCategory();
    QString getAuthor();
    QString getTitle();
    QString getDescription();
    QIcon getCategoryIcon();
    QString getImage();
private:
    int category;
    QString author;
    QString title;
    QString description;
    QString image;
};

#endif // EPISODE_H
