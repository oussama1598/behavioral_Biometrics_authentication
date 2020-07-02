#pragma once

#include <string>
#include <sstream>
#include <vector>

class StringsHelpers {
public:
    static std::vector<std::string> split(std::string &string, char delimiter = ' ');
};

