#pragma once

#include <algorithm>
#include <string>
#include <sstream>
#include <vector>

class StringsHelpers {
public:
    static std::vector<std::string> split(const std::string &string, char delimiter = ' ');

    static std::string removeMultipleSpaces(const std::string &s);

    static int hexStringToInt(const std::string &s);

    static int hexStringUnsignedToInt(const std::string &s);

    static int stringToInt(const std::string &s);

    static long stringToLongInt(const std::string &s);
};

