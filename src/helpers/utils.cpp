#include "utils.h"

std::string Utils::getCurrentTimeStamp() {
    std::time_t result = std::time(nullptr);

    return std::to_string(result);
}

std::string Utils::getCurrentTime() {
    std::time_t result = std::time(nullptr);

    return std::asctime(std::localtime(&result));
}
