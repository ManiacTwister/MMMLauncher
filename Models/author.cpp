#include "author.h"
Author::Author()
{

}

Author::Author(int uid, QString name, int gameCount)
{
    setUid(uid);
    setName(name);
    setGameCount(gameCount);
}

void Author::setUid(int newId) { uid = newId; }
void Author::setName(QString newName) { name = newName; }
void Author::setGameCount(int newGameCount) { gameCount = newGameCount; }

int Author::getUid() { return uid; }
QString Author::getName() { return name; }
int Author::getGameCount() { return gameCount; }
