#pragma once

#include <iostream>
#include <map>
#include <fstream>

#include "helpers/file_logger.h"
#include "helpers/strings_helpers.h"
#include "helpers/utils.h"

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

    struct Touch {
        long timestamp{-1};
        int x{-1}, y{-1};
    };

    struct TouchDistribution {
        int count{0};
        double speed{0};
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

        std::array<int, 9> distrubtionsOfTouchs{};

        long totalMovements{0};
        std::array<TouchDistribution, 9> touchDirections{{
                                                                 {},
                                                                 {},
                                                                 {},
                                                                 {},
                                                                 {},
                                                                 {},
                                                                 {},
                                                                 {},
                                                                 {}
                                                         }};

        std::array<int, 9> travelDistanceDistributions{};

        std::array<double, 9> extremeSpeeds{};

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

    std::map<int, Touch> _firstTouches;
    std::map<int, Touch> _lastTouches;

private:
    void _loadLogs(std::map<long, std::string> &logValues, const std::string &filename,
                   const std::string &logName);

    void _parseKeyboardLog(Slice &slice, std::vector<std::string> &parsedOutput);

    void _parseOrientationLog(Slice &slice, std::vector<std::string> &parsedOutput) const;

    void
    _addTouchData(DataParser::Slice &slice, const Touch &firstTouch, const Touch &lastTouch) const;

    void _parseTouchLog(Slice &slice, std::vector<std::string> &parsedOutput);

public:

    void combineLogs(const std::string &orientationFilename, const std::string &keyboardFilename,
                     const std::string &touchFilename);

    void getSlices();

    void _averageKeystrokeData();
};

