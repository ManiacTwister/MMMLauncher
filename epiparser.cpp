#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>
#include "epiparser.h"
#include "category.h"
#include "episode.h"

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
        QJsonArray epis = ob["MMMLauncher"].toObject()["MMMInfo"].toArray();
        episodes.reserve(epis.count());

        foreach (const QJsonValue &epi, epis) {
            QJsonObject epiObject = epi.toObject();

            QString title = "Unbekannt";
            QString author = "Unbekannt";
            QString description = "- Keine Beschreibung verfügbar -";
            QString screenshot = "noscreen";
            int type = Category::Episode;
            QString archive = "noarchive";


            if(!epiObject["Title"].isNull()) {
                title = epiObject["Title"].toString();
            }
            if(!epiObject["Author"].isNull()) {
                author = epiObject["Author"].toString();
            }
            if(!epiObject["Description"].isNull()) {
                description = epiObject["Description"].toString();
            }
            if(!epiObject["Type"].isNull()) {
                type = Category::smallToCategory(epiObject["Type"].toString());
            }
            if(!epiObject["Screenshot"].isNull()) {
                screenshot = epiObject["Screenshot"].toString();
            }
            if(!epiObject["Archive"].isNull()) {
                archive = epiObject["Archive"].toString();
            }

            episodes.push_back(Episode(
                title,
                description,
                author,
                type,
                screenshot,
                archive
            ));
       }
    } else {
        qWarning() << parseError.errorString();
    }
}
