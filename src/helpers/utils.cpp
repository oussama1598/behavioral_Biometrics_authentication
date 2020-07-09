#include "utils.h"

std::string Utils::getCurrentTimeStamp() {
    std::chrono::milliseconds result = std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now().time_since_epoch()
    );

    return std::to_string(result.count());
}

std::string Utils::getCurrentTime() {
    std::time_t result = std::time(nullptr);

    return std::asctime(std::localtime(&result));
}
