#ifndef EPIPARSER_H
#define EPIPARSER_H

#include <QObject>
#include <QByteArray>
#include <QMap>
#include "Models/episode.h"

class EpiParser : public QObject
{
    Q_OBJECT
public:
    EpiParser();
    explicit EpiParser(QByteArray data, QObject *parent = 0);
    QMap <int, Episode*> episodes;
    void parse();
    bool parsed = false;
signals:

private:
    QByteArray data;
    
public slots:
    
};

#endif // EPIPARSER_H
