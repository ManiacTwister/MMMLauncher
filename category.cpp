#include "category.h"
#include <QString>

Category::Category()
{
}

int Category::smallToCategory(QString type)
{
    if(type == "epi")
        return Category::Episode;
    else if(type == "sp")
        return Category::Special;
    else if(type == "td")
        return Category::TrailerDemo;
    else if(type == "tr")
        return Category::Trash;
}
