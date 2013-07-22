#ifndef CATEGORY_H
#define CATEGORY_H

#include <QString>

class Category
{
public:
    Category();
    static const int Episode = 0;
    static const int Special = 1;
    static const int TrailerDemo = 2;
    static const int Trash = 3;
    static int smallToCategory(QString type);
};

static QString categories[4] = { "Episoden", "Specials", "Trailer & Demos", "Trash" };
#endif // CATEGORY_H
