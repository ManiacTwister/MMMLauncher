#include "episode.h"
#include <QString>
#include <QGraphicsScene>
#include "category.h"
#include <QDebug>
Episode::Episode()
{
}

Episode::Episode(QString title, QString description, QString author, int category, QString image)
{
    setTitle(title);
    setDescription(description);
    setAuthor(author);
    setCategory(category);
    setImage(image);
}

void Episode::setTitle(QString newtitle)
{
    title = newtitle;
}

void Episode::setDescription(QString newdesc)
{
    description = newdesc;
}

void Episode::setAuthor(QString newauthor)
{
    author = newauthor;
}

void Episode::setCategory(int newcategory)
{
    category = newcategory;
}

void Episode::setImage(QString newimage)
{
    image = newimage;
}

QString Episode::getTitle() { return title; }
QString Episode::getDescription() { return description; }
QString Episode::getAuthor() { return author; }
int Episode::getCategory() { return category; }
QIcon Episode::getCategoryIcon()
{
    switch(getCategory())
    {
        case 0:
            return QIcon(":/images/images/epi.ico");
        case 1:
            return QIcon(":/images/images/sp.ico");
        case 2:
            return QIcon(":/images/images/td.ico");
        case 3:
            return QIcon(":/images/images/tr.ico");
    }
    return QIcon(":/images/images/epi.ico");
}
QString Episode::getImage() { return image; }
