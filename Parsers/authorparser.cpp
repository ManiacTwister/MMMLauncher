#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>
#include "authorparser.h"
#include "Models/author.h"

AuthorParser::AuthorParser()
{
    parsed = false;
}

AuthorParser::AuthorParser(QByteArray downloadeddata, QObject *parent) :
    QObject(parent)
{
    data = downloadeddata;
}

void AuthorParser::parse()
{
    QJsonParseError  parseError;
    QJsonDocument json = QJsonDocument::fromJson(data, &parseError);


    if(parseError.error == QJsonParseError::NoError)
    {

        QJsonObject ob = json.object();
        QJsonArray authorsj = ob["objects"].toArray();
        //authors.reserve(authorsj.count());
        foreach (const QJsonValue &author, authorsj) {
            QJsonObject authorObject = author.toObject();
            int authorUid = (int) authorObject["ID"].toDouble();
            authors.insert(authorUid, new Author(authorUid, authorObject["Name"].toString(), authorObject["Games"].toArray().count()));
       }
    } else {
        qWarning() << "Authorparser: " << parseError.errorString();
    }

    parsed = true;
}
