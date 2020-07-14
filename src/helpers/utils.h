#pragma once

#include <string>
#include <ctime>
#include <chrono>
#include <cmath>

class Utils {
public:
    static std::string getCurrentTimeStamp();

    static std::string getCurrentTime();

    static double getDistance(int x1, int y1, int x2, int y2);

    static double getVectorDirection(int x1, int y1, int x2, int y2);
};

