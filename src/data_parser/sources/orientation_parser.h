#pragma once

#include <iostream>
#include <array>
#include <vector>
#include <string>
#include <fstream>

#include "helpers/strings_helpers.h"

class OrientationParser {
private:
    struct State {
        int count{0};
        int timeSpent{0};
    };

    struct Slice {
        bool done{false};
        int duration{0};
        int lastTimestamp{-1};

        std::array<State, 4> states
                {
                        {
                                {0, 0},
                                {0, 0},
                                {0, 0},
                                {0, 0}
                        }
                };
    };

    long int _totalTime{0};
    int _lastStateId{-1};

    std::vector<Slice> _slices;

public:
    explicit OrientationParser(const std::string &filename);
};

