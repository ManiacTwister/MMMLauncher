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
        QJsonArray epis = ob["objects"].toArray();
        //episodes.reserve(epis.count());

        foreach (const QJsonValue &epi, epis) {
            QJsonObject epiObject = epi.toObject();
            int id = -1;
            QString title = tr("Unbekannt");
            int authorUid = -1;
            QString description = tr("- Keine Beschreibung verfügbar -");
            QString screenshotUrl = QString();
            int categoryUid = -1;
            QString downloadUrl = QString();

            if(!epiObject["ID"].isNull()) {
                id = (int) epiObject["ID"].toDouble();
            }
            if(!epiObject["Title"].isNull()) {
                title = epiObject["Title"].toString();
            }
            if(!epiObject["Author_id"].isNull()) {
                authorUid = (int) epiObject["Author_id"].toDouble();
            }
            if(!epiObject["Description"].isNull()) {
                description = epiObject["Description"].toString();
            }
            if(!epiObject["Category_id"].isNull()) {
                categoryUid = (int) epiObject["Category_id"].toDouble();
            }
            if(!epiObject["Screenshot"].isNull()) {
                screenshotUrl = epiObject["Screenshot"].toString();
            }
            if(!epiObject["Download"].isNull()) {
                downloadUrl = epiObject["Download"].toString();
            }
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
        qWarning() << "Epiparser: " << parseError.errorString();
    }

    parsed = true;
}
