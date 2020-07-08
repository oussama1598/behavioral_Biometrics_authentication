#pragma once


#include "sources/orientation_parser.h"

class DataParser {
private:
    OrientationParser _orientationParser{"test"};

public:
    void getSlices();
};

