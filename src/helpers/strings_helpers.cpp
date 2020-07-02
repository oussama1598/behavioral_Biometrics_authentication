#include "strings_helpers.h"

std::vector<std::string> StringsHelpers::split(std::string &string, char delimiter) {
    std::vector<std::string> tokens;

    std::stringstream ss(string);
    std::string token;

    while (std::getline(ss, token, delimiter)) {
        tokens.push_back(token);
    }

    return tokens;
}
