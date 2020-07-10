#pragma once

#include <iostream>
#include <map>
#include <fstream>

#include "helpers/file_logger.h"
#include "helpers/strings_helpers.h"

class DataParser {
private:
    struct Key {
        int count{0};
        long duration{0};
        long pressedTimestamp{0};
    };

    struct State {
        int count{0};
        long lastTimestamp{-1};
        long timeSpent{0};
    };

    struct Slice {
        bool done{false};
        long duration{0};
        long lastTimestamp{-1};

        std::map<int, Key> keys{};

        std::array<State, 4> states
                {
                        {
                                {0, -1, 0},
                                {0, -1, 0},
                                {0, -1, 0},
                                {0, -1, 0}
                        }
                };
    };

    long _totalTime{0};

    std::vector<Slice> _slices;

    int _lastStateId{-1};

private:
    void _loadLogs(std::map<long, std::string> &logValues, const std::string &filename,
                   const std::string &logName);

    void _parseKeyboardLog(Slice &slice, std::vector<std::string> &parsedOutput);

    void _parseOrientationLog(Slice &slice, std::vector<std::string> &parsedOutput);

public:

    void combineLogs(const std::string &orientationFilename, const std::string &keyboardFilename);

    void getSlices();
};

