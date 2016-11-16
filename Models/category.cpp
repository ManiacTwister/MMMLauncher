#include "category.h"
#include <QString>
#include <QIcon>

Category::Category()
{

}

Category::Category(int uid, QString title, QString icon)
{
    setUid(uid);
    setTitle(title);
    setIcon(icon);
}
Category::Category(int uid, int parentUid, QString title, QIcon icon)
{
    setUid(uid);
    setParentUid(parentUid);
    setTitle(title);
    setIcon(icon);
}

int Category::getUid() { return uid; }
int Category::getParentUid() { return parentUid; }
QString Category::getTitle() { return title; }
QIcon Category::getIcon() { return icon; }

void Category::setUid(int newId) { uid = newId; }
void Category::setParentUid(int newParentId) { parentUid = newParentId; }
void Category::setTitle(QString newTitle) { title = newTitle; }

void Category::setIcon(QString name) { icon = QIcon(":/images/images/"+name+".png"); }
void Category::setIcon(QIcon name) { icon = name; }
