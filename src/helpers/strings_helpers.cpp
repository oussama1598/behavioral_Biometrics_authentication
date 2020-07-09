#include "strings_helpers.h"

std::vector<std::string> StringsHelpers::split(const std::string &string, char delimiter) {
    std::vector<std::string> tokens;

    std::stringstream ss(string);
    std::string token;

    while (std::getline(ss, token, delimiter)) {
        tokens.push_back(token);
    }

    return tokens;
}

std::string StringsHelpers::removeMultipleSpaces(const std::string &s) {
    std::string newString;

    size_t currentCharIndex = 0;
    size_t stringLength = s.length();

    while (currentCharIndex < stringLength) {
        size_t end = s.find_first_of(' ', currentCharIndex) + 1;

        newString += s.substr(currentCharIndex, end - currentCharIndex);

        if (s.substr(currentCharIndex, stringLength).find(' ') != std::string::npos) {
            currentCharIndex = s.find_first_not_of(' ', end);
        } else {
            currentCharIndex = stringLength;
        }
    }

    return newString;
}

int StringsHelpers::hexStringToInt(const std::string &s) {
    int value;

    std::stringstream ss;

    ss << std::hex << s;
    ss >> value;

    return value;
}

int StringsHelpers::hexStringUnsignedToInt(const std::string &s) {
    unsigned int value;

    std::stringstream ss;

    ss << std::hex << s;
    ss >> value;

    return static_cast<int>(value);
}

int StringsHelpers::stringToInt(const std::string &s) {
    return std::stoi(s);
}

long StringsHelpers::stringToLongInt(const std::string &s) {
    return std::stol(s);
}
