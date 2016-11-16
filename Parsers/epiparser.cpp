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
            int actions = -1;
            int length = -1;
            int level = -1;
            int edgar = -1;
            QStringList languageList;

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
            if(!epiObject["Actions"].isNull()) {
                actions = epiObject["Actions"].toInt();
            }
            if(!epiObject["Length"].isNull()) {
                length = epiObject["Length"].toInt();
            }
            if(!epiObject["Level"].isNull()) {
                level = epiObject["Level"].toInt();
            }
            if(!epiObject["LanguageList"].isNull()) {
                foreach(QJsonValue language, epiObject["LanguageList"].toArray() ) {
                   if(language == "de" || language == "") continue;
                   languageList << language.toString();
                }

            }
            if(!epiObject["Edgar"].isNull()) {
                edgar = epiObject["Edgar"].toInt();
            }

            episodes.insert(id, new Episode(
                id,
                title,
                description,
                authorUid,
                categoryUid,
                screenshotUrl,
                downloadUrl,
                actions,
                length,
                level,
                languageList,
                edgar
            ));
       }
    } else {
        qWarning() << "Epiparser: " << parseError.errorString();
    }

    parsed = true;
}
