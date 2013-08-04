#ifndef CATEGORY_H
#define CATEGORY_H

#include <QString>
#include <QIcon>

class Category
{
private:
    int uid;
    int parentUid = -1;
    QString title;
    QIcon icon;
public:
    Category();
    Category(int uid, QString title, QString icon);
    Category(int uid, int parentUid, QString title, QString icon);

    int getUid();
    int getParentUid();
    QString getTitle();
    QIcon getIcon();

    void setUid(int newId);
    void setParentUid(int newParentId);
    void setTitle(QString newTitle);
    void setIcon(QString name);
};

#endif // CATEGORY_H
