#ifndef CATEGORYPARSER_H
#define CATEGORYPARSER_H

#include <QObject>
#include <QByteArray>
#include <QMap>
#include "Models/category.h"

class CategoryParser : public QObject
{
    Q_OBJECT
public:
    CategoryParser();
    explicit CategoryParser(QByteArray data, QObject *parent = 0);
    QMap <int, Category*> categories;
    QMap <QString, QString> categoryIcons;
    void parse();
    bool parsed = false;
signals:

private:
    QByteArray data;

public slots:

};

#endif // CATEGORYPARSER_H
