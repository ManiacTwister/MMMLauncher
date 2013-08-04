#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>
#include "epiparser.h"
#include "Models/category.h"
#include "Models/episode.h"

EpiParser::EpiParser()
{
    parsed = false;
}


EpiParser::EpiParser(QByteArray downloadeddata, QObject *parent) :
    QObject(parent)
{
    data = downloadeddata;
}

void EpiParser::parse()
{
    QJsonParseError  parseError;
    QJsonDocument json = QJsonDocument::fromJson(data, &parseError);


    if(parseError.error == QJsonParseError::NoError)
    {

        QJsonObject ob = json.object();
        QJsonArray epis = ob["gameinfos"].toArray();
        //episodes.reserve(epis.count());

        foreach (const QJsonValue &epi, epis) {
            QJsonObject epiObject = epi.toObject();
            int id;
            QString title = tr("Unbekannt");
            int authorUid = -1;
            QString description = tr("- Keine Beschreibung verfügbar -");
            QString screenshotUrl = "http://launcher.maniactwister.de/noscreen.png";
            int categoryUid = -1;
            QString downloadUrl = "nodownload";

            if(!epiObject["id"].isNull()) {
                id = (int) epiObject["id"].toDouble();
            }
            if(!epiObject["title"].isNull()) {
                title = epiObject["title"].toString();
            }
            if(!epiObject["authorid"].isNull()) {
                authorUid = (int) epiObject["authorid"].toDouble();
            }
            if(!epiObject["desc"].isNull()) {
                description = epiObject["desc"].toString();
            }
            if(!epiObject["categoryid"].isNull()) {
                categoryUid = (int) epiObject["categoryid"].toDouble();
            }
            if(!epiObject["screenshot"].isNull()) {
                screenshotUrl = epiObject["screenshot"].toString();
            }
            if(!epiObject["download"].isNull()) {
                downloadUrl = epiObject["download"].toString();
            }

            episodes.insert(id, new Episode(
                id,
                title,
                description,
                authorUid,
                categoryUid,
                screenshotUrl,
                downloadUrl
            ));
       }
    } else {
        qWarning() << parseError.errorString();
    }

    parsed = true;
}
