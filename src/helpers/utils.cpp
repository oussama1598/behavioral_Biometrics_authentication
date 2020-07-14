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

double Utils::getDistance(int x1, int y1, int x2, int y2) {
    return std::sqrt(std::pow(x2 - x1, 2) + (int) std::pow(y2 - y1, 2));
}

double Utils::getVectorDirection(int x1, int y1, int x2, int y2) {
    double dotProduct = y2 - y1;
    double crossProduct = -(x2 - x1);

    double angle = std::atan2(crossProduct, dotProduct) * (180 / M_PI);

    if (angle < 0)
        return angle * -1;

    angle = 180 - angle;

    return angle + 180;
}
