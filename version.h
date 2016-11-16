#ifndef VERSION_H
#define VERSION_H
#include <cstdio>
#include <string>
#include <iostream>

struct Version
{
    int major, minor, revision;

    Version(const std::string& version)
    {
        std::sscanf(version.c_str(), "%d.%d.%d", &major, &minor, &revision);
        if (major < 0) major = 0;
        if (minor < 0) minor = 0;
        if (revision < 0) revision = 0;
    }

    bool operator < (const Version& other)
    {
        if (major < other.major)
            return true;
        if (minor < other.minor)
            return true;
        if (revision < other.revision)
            return true;
        return false;
    }

    bool operator <= (const Version& other)
    {
        if (major < other.major)
            return true;
        if (minor < other.minor)
            return true;
        if (revision <= other.revision)
            return true;
        return false;
    }

    bool operator == (const Version& other)
    {
        return major == other.major
            && minor == other.minor
            && revision == other.revision;
    }

    friend std::ostream& operator << (std::ostream& stream, const Version& ver)
    {
        stream << ver.major;
        stream << '.';
        stream << ver.minor;
        stream << '.';
        stream << ver.revision;
        return stream;
    }
};

#endif // VERSION_H
