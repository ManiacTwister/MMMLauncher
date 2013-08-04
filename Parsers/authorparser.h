#ifndef AUTHORPARSER_H
#define AUTHORPARSER_H

#include <QObject>
#include <QByteArray>
#include <QMap>
#include "Models/author.h"

class AuthorParser : public QObject
{
    Q_OBJECT
public:
    AuthorParser();
    explicit AuthorParser(QByteArray data, QObject *parent = 0);
    QMap <int, Author*> authors;
    void parse();
    bool parsed = false;
signals:

private:
    QByteArray data;

public slots:

};
#endif // AUTHORPARSER_H
