#include "data_parser.h"

void DataParser::_loadLogs(std::map<long, std::string> &logValues, const std::string &filename,
                           const std::string &logName) {
    std::fstream logs;

    logs.open(filename);

    if (!logs.is_open())
        throw std::runtime_error("Could not open file.");

    std::string lineBuffer;

    while (std::getline(logs, lineBuffer)) {
        std::vector<std::string> parsedOutput = StringsHelpers::split(lineBuffer, ' ');
        long timestamp = StringsHelpers::stringToLongInt(parsedOutput[0]);

        logValues.insert({timestamp, logName + " " + lineBuffer});
    }

    logs.close();
}

void
DataParser::_parseKeyboardLog(DataParser::Slice &slice, std::vector<std::string> &parsedOutput) {
    long timestamp = StringsHelpers::stringToLongInt(parsedOutput[1]);
    std::string keyAction = parsedOutput[2];
    int keyCode = StringsHelpers::stringToInt(parsedOutput[3]);

    if (keyAction == "press") {
        slice.keys[keyCode].pressedTimestamp = timestamp;
        slice.keys[keyCode].count += 1;
    }

    if (keyAction == "release") {
        long deltaTime = timestamp - slice.keys[keyCode].pressedTimestamp;

        slice.keys[keyCode].duration += deltaTime;
    }
}

void
DataParser::_parseOrientationLog(DataParser::Slice &slice, std::vector<std::string> &parsedOutput) {
    long timestamp = StringsHelpers::stringToLongInt(parsedOutput[1]);
    int stateId = StringsHelpers::stringToInt(parsedOutput[2]);

    auto &state = slice.states[stateId];

    if (state.lastTimestamp != -1) {
        long deltaTime = timestamp - state.lastTimestamp;

        slice.states[stateId].timeSpent += deltaTime;
    }

    state.lastTimestamp = timestamp;

    if (_lastStateId != -1 && _lastStateId != stateId) {
        slice.states[stateId].count += 1;
    }
}

void DataParser::combineLogs(const std::string &orientationFilename,
                             const std::string &keyboardFilename) {
    FileLogger combinedlogs{"data/combined.txt"};
    std::map<long, std::string> logValues;


    _loadLogs(logValues, orientationFilename, "ORIENTATION");
    _loadLogs(logValues, keyboardFilename, "KEYBOARD");

    for (auto &line : logValues) {
        combinedlogs.addLine(line.second);
    }
}

void DataParser::getSlices() {
    std::fstream fileStream;
    std::string lineBuffer;

    fileStream.open("data/combined.txt");

    if (!fileStream.is_open())
        throw std::runtime_error("File Not Opened");

    _slices.push_back({false, 0, -1});

    while (std::getline(fileStream, lineBuffer)) {
        std::vector<std::string> parsedOutput = StringsHelpers::split(lineBuffer, ' ');

        std::string logName = parsedOutput[0];
        long timestamp = StringsHelpers::stringToLongInt(parsedOutput[1]);

        for (auto &slice: _slices) {

            if (slice.done)
                continue;

            if (slice.lastTimestamp != -1) {
                long deltaTime = timestamp - slice.lastTimestamp;

                _totalTime += deltaTime;
                slice.duration += deltaTime;
            }

            slice.lastTimestamp = timestamp;

            if (slice.duration >= 60 * 5 * 1000) // 5 minutes
                slice.done = true;

            if (logName == "KEYBOARD")
                _parseKeyboardLog(slice, parsedOutput);

            if (logName == "ORIENTATION")
                _parseOrientationLog(slice, parsedOutput);


            // add a slice every 90 seconds
            if ((size_t) (_totalTime / (90 * 1000)) == _slices.size()) {
                _slices.push_back({false, 0, -1});
            }
        }
    }
}
