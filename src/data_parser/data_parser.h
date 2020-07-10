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
        long duration{INT32_MAX};
        long pressedTimestamp{0};
    };

    struct State {
        int count{0};
        long lastTimestamp{-1};
        long timeSpent{0};
    };

    struct Slice {
        bool done{false};
        bool next{true};
        long duration{0};
        long lastTimestamp{-1};

        // keys data
        std::map<int, Key> keys{};
        long keyLatency{INT32_MAX};
        long lastKeyTimestamp{-1};

        std::array<State, 4> states
                {
                        {
                                {0, -1, 0},
                                {0, -1, 0},
                                {0, -1, 0},
                                {0, -1, 0}
                        }
                };

        std::array<int, 9> distrubtionsOfTouchs{
                0, 0, 0, 0, 0, 0, 0, 0, 0
        };

        inline Slice() {
            for (int i = 0; i < 26; ++i) {
                keys.insert({65 + i, {}});
                keys.insert({97 + i, {}});
            }
        }
    };

    std::vector<Slice> _slices;

    int _lastStateId{-1};
    int _deviceWidth{0};
    int _deviceHeight{0};

private:
    void _loadLogs(std::map<long, std::string> &logValues, const std::string &filename,
                   const std::string &logName);

    void _parseKeyboardLog(Slice &slice, std::vector<std::string> &parsedOutput);

    void _parseOrientationLog(Slice &slice, std::vector<std::string> &parsedOutput) const;

    void _parseTouchLog(Slice &slice, std::vector<std::string> &parsedOutput);

public:

    void combineLogs(const std::string &orientationFilename, const std::string &keyboardFilename,
                     const std::string &touchFilename);

    void getSlices();

    void _averageKeystrokeData();
};

