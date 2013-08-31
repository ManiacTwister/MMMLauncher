#ifndef VERSION_H
#define VERSION_H

#include <QObject>
#include <iostream>
#include <sstream>
#include <vector>
#include <iterator>

class Version : public QObject
{
    Q_OBJECT
    struct Dig
    {
        int value;
        operator int() const {return value;}
    };
    friend std::istream& operator>>(std::istream& str, Version::Dig& dig);

    public:
        Version(std::string const&);
        bool operator<(Version const&) const;

    private:
        std::vector<int> vInfo;
};

#endif // VERSION_H
