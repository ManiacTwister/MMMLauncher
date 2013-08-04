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


    if(parseError.error == QJsonParseError::NoError)
    {

        QJsonObject ob = json.object();
        QJsonArray categoriesj = ob["categorytree"].toArray();
        //categories.reserve(categoriesj.count());
        foreach (const QJsonValue &category, categoriesj) {

            QJsonObject categoryObject = category.toObject();
            int parentUid = (int) categoryObject["id"].toDouble();
            QString parentIcon = categoryObject["icon"].toString();

            categories.insert(parentUid, new Category(parentUid, categoryObject["title"].toString(), parentIcon));
            if(!categoryObject["categories"].isNull()) {
                foreach (const QJsonValue &subCategory, categoryObject["categories"].toArray()) {
                    QJsonObject subCategoryObject = subCategory.toObject();
                    int subCategoryUid = (int) subCategoryObject["id"].toDouble();
                    categories.insert(subCategoryUid, new Category(
                                                         subCategoryUid,
                                                         parentUid,
                                                         subCategoryObject["title"].toString(),
                                                         parentIcon
                                                     ));
                }
            }
       }
    } else {
        qWarning() << parseError.errorString();
    }

    parsed = true;
}
