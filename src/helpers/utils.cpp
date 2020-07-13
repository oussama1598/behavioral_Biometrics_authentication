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

int Utils::getDistanceSquared(int x1, int y1, int x2, int y2) {
    return (int) std::pow(x2 - x1, 2) + (int) std::pow(y2 - y1, 2);
}

double Utils::getVectorDirection(int x1, int y1, int x2, int y2) {
    if (x1 == x2) return 0;

    return (std::atan((y2 - y1) / (x2 - x1)) * (180 / M_PI)) + 90;
}
