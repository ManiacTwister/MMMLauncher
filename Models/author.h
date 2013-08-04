#ifndef AUTHOR_H
#define AUTHOR_H

#include <QString>

class Author
{
private:
    QString name;
    int uid;
    int gameCount;
public:
    Author();
    Author(int uid, QString name, int gameCount);
    int getUid();
    QString getName();
    int getGameCount();
    void setUid(int newId);
    void setName(QString newName);
    void setGameCount(int newGameCount);
};

#endif // AUTHOR_H
