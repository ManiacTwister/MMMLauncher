#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>
#include "categoryparser.h"
#include "Models/category.h"

CategoryParser::CategoryParser()
{
    parsed = false;
}

CategoryParser::CategoryParser(QByteArray downloadeddata, QObject *parent) :
    QObject(parent)
{
    data = downloadeddata;
}

void CategoryParser::parse()
{
    QJsonParseError  parseError;
    QJsonDocument json = QJsonDocument::fromJson(data, &parseError);


    // New api doesn't know about icons
    categoryIcons["Episoden"] = "chuck";
    categoryIcons["Specials"] = "redchuck";
    categoryIcons["Fan Adventures"] = "greenchuck";
    categoryIcons["Trailer & Demos"] = "bluechuck";
    categoryIcons["Underground"] = "lilachuck";

    if(parseError.error == QJsonParseError::NoError)
    {

        QJsonObject ob = json.object();
        QJsonArray categoriesj = ob["objects"].toArray();
        //categories.reserve(categoriesj.count());
        foreach (const QJsonValue &category, categoriesj) {

            QJsonObject categoryObject = category.toObject();
            int Uid = (int) categoryObject["ID"].toDouble();
            QString Icon = categoryIcons[categoryObject["Title"].toString()];
            QString Title = categoryObject["Title"].toString();

            if(!categoryObject["ParentID"].isNull()) {
                int parentUid = (int) categoryObject["ParentID"].toDouble();
                    categories.insert(Uid, new Category(
                                                         Uid,
                                                         parentUid,
                                                         Title,
                                                         categories[parentUid]->getIcon()
                                                     ));
            } else {
                categories.insert(Uid, new Category(Uid, Title, Icon));
            }
       }
    } else {
        qWarning() << "Categoryparser: " << parseError.errorString();
    }

    parsed = true;
}
