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

    if (
            (keyCode < 65 || keyCode > 90) &&
            (keyCode < 97 || keyCode > 122)
            )
        return;

    if (keyAction == "press") {
        slice.keys[keyCode].pressedTimestamp = timestamp;
        slice.keys[keyCode].count += 1;

        if (slice.lastKeyTimestamp != -1) {
            long deltaTime = timestamp - slice.lastKeyTimestamp;

            if (deltaTime < (3 * 1000))
                slice.keyLatency += timestamp - slice.lastKeyTimestamp;
        }
    }

    if (keyAction == "release") {
        long deltaTime = timestamp - slice.keys[keyCode].pressedTimestamp;

        slice.keys[keyCode].duration += deltaTime;

        slice.lastKeyTimestamp = timestamp;

        if (slice.keyLatency == INT32_MAX) {
            slice.keyLatency = 0;
        }
    }
}

void
DataParser::_parseOrientationLog(DataParser::Slice &slice,
                                 std::vector<std::string> &parsedOutput) const {
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

void DataParser::_parseTouchLog(DataParser::Slice &slice, std::vector<std::string> &parsedOutput) {
    slice.lastKeyTimestamp = -1;

    std::cout << slice.duration << " " << parsedOutput[0] << std::endl;
}


void DataParser::combineLogs(const std::string &orientationFilename,
                             const std::string &keyboardFilename,
                             const std::string &touchFilename) {
    FileLogger combinedlogs{"data/combined.txt"};
    std::map<long, std::string> logValues;


    _loadLogs(logValues, orientationFilename, "ORIENTATION");
    _loadLogs(logValues, keyboardFilename, "KEYBOARD");
    _loadLogs(logValues, touchFilename, "TOUCH");

    for (auto &line : logValues) {
        combinedlogs.addLine(line.second, false);
    }
}

void DataParser::getSlices() {
    std::fstream fileStream;
    std::string lineBuffer;

    fileStream.open("data/combined.txt");

    if (!fileStream.is_open())
        throw std::runtime_error("File Not Opened");

    _slices.emplace_back(Slice());

    while (std::getline(fileStream, lineBuffer)) {
        std::vector<std::string> parsedOutput = StringsHelpers::split(lineBuffer, ' ');

        std::string logName = parsedOutput[0];
        long timestamp = StringsHelpers::stringToLongInt(parsedOutput[1]);

        for (auto &slice: _slices) {
            if (slice.done)
                continue;

            if (slice.lastTimestamp != -1) {
                long deltaTime = timestamp - slice.lastTimestamp;

                slice.duration += deltaTime;
            }

            slice.lastTimestamp = timestamp;

            if (slice.duration >= 60 * 5 * 1000) // 5 minutes
                slice.done = true;

            if (logName == "KEYBOARD")
                _parseKeyboardLog(slice, parsedOutput);

            if (logName == "ORIENTATION")
                _parseOrientationLog(slice, parsedOutput);

            if (logName == "TOUCH")
                _parseTouchLog(slice, parsedOutput);

            // add a slice every 90 seconds
            if (slice.duration > (90 * 1000) && slice.next) {
                _slices.emplace_back(Slice());

                slice.next = false;
            }
        }
    }

    fileStream.close();

    std::cout << "ok" << std::endl;
}
