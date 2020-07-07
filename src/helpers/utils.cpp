#include "utils.h"

std::string Utils::getCurrentTimeStamp() {
    std::time_t result = std::time(nullptr);

    return std::to_string(result);
}
