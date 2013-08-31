#include "version.h"

Version::Version(std::string const& versionStr)
{
    std::stringstream versionStream(std::string(".") + versionStr);

    std::copy(std::istream_iterator<Dig>(versionStream), std::istream_iterator<Dig>(), std::back_inserter(vInfo));
}

bool Version::operator<(Version const& rhs) const
{
    return std::lexicographical_compare(vInfo.begin(), vInfo.end(), rhs.vInfo.begin(), rhs.vInfo.end());
}

std::istream& operator>>(std::istream& str, Version::Dig& digit)
{
    str.get();
    str >> digit.value;
    return str;
}
