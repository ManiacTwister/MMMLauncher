#ifndef EPIPARSER_H
#define EPIPARSER_H

#include <QObject>
#include <QByteArray>
#include "episode.h"

class EpiParser : public QObject
{
    Q_OBJECT
public:
    explicit EpiParser(QByteArray data, QObject *parent = 0);
    std::vector<Episode> episodes;
    void parse();
signals:

private:
    QByteArray data;
    
public slots:
    
};

#endif // EPIPARSER_H
