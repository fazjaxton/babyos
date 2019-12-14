#ifndef UTIL_H
#define UTIL_H

#include <algorithm>

/*******************************************************************/
inline void toLower(std::string & str)
{
    std::transform(str.begin(), str.end(), str.begin(),
            [](unsigned char c) { return std::tolower(c); });
}

#endif
