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
            episodes.push_back(Episode(
                epi.toObject()["Title"].toString(),
                epi.toObject()["Description"].toString(),
                epi.toObject()["Author"].toString(),
                Category::smallToCategory(epi.toObject()["Type"].toString()),
                epi.toObject()["Screenshot"].toString()
            ));
       }
    } else {
        qWarning() << parseError.errorString();
    }
}
