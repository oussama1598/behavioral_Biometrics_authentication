#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <map>

#include "helpers/strings_helpers.h"

class KeyboardParser {
private:
    struct Key {
        int count{0};
        int duration{0};
        long pressedTimestamp{0};
    };

    struct Slice {
        bool done{false};
        int duration{0};
        long lastTimestamp{-1};

        std::map<int, Key> keys{
                {104, {}},
                {103, {}},
        };
    };

    long _totalTime{0};

    std::vector<Slice> _slices;

public:
    explicit KeyboardParser(const std::string &filename);
};

