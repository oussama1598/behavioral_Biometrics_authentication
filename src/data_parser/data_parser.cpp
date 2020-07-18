#include "data_parser.h"

DataParser::DataParser(const std::string &name) : _name(name) {}

void DataParser::_loadLogs(std::map<long, std::string> &logValues, const std::string &filename,
                           const std::string &logName) {
    std::fstream logs;

    logs.open(filename);

    if (!logs.is_open())
        throw std::runtime_error("Could not open file.");

    std::string lineBuffer;

    while (std::getline(logs, lineBuffer)) {
        std::vector<std::string> parsedOutput = StringsHelpers::split(lineBuffer, ' ');
        long timestamp = StringsHelpers::stringToLongInt(parsedOutput.at(0));

        logValues.insert({timestamp, logName + " " + lineBuffer});
    }

    logs.close();
}

void
DataParser::_parseKeyboardLog(DataParser::Slice &slice, std::vector<std::string> &parsedOutput) {
    long timestamp = StringsHelpers::stringToLongInt(parsedOutput.at(1));
    std::string keyAction = parsedOutput.at(2);
    int keyCode = StringsHelpers::stringToInt(parsedOutput.at(3));

    if (
            (keyCode < 65 || keyCode > 90) &&
            (keyCode < 97 || keyCode > 122)
            )
        return;

    auto &key = slice.keys.at(keyCode);

    if (keyAction == "press") {
        key->pressedTimestamp = timestamp;
        key->count += 1;

        if (slice.lastKeyTimestamp != -1) {
            long deltaTime = timestamp - slice.lastKeyTimestamp;

            if (deltaTime < (3 * 1000))
                slice.keyLatency += timestamp - slice.lastKeyTimestamp;
        }
    }

    if (keyAction == "release") {
        long deltaTime = timestamp - key->pressedTimestamp;

        if (slice.keys.at(keyCode)->duration == INT32_MAX)
            key->duration = 0;

        key->duration += deltaTime;

        slice.lastKeyTimestamp = timestamp;

        if (slice.keyLatency == INT32_MAX) {
            slice.keyLatency = 0;
        }
    }
}

void
DataParser::_parseOrientationLog(DataParser::Slice &slice,
                                 std::vector<std::string> &parsedOutput) {
    long timestamp = StringsHelpers::stringToLongInt(parsedOutput.at(1));
    int stateId = StringsHelpers::stringToInt(parsedOutput.at(2));

    auto &state = slice.states.at(stateId);

    if (state.lastTimestamp != -1) {
        long deltaTime = timestamp - state.lastTimestamp;

        slice.states.at(stateId).timeSpent += deltaTime;
    }

    state.lastTimestamp = timestamp;

    if (_lastStateId != -1 && _lastStateId != stateId) {
        slice.states.at(stateId).count += 1;
    }

    if (_lastStateId == -1) {
        slice.states.at(stateId).count += 1;
    }

    _lastStateId = stateId;
}

void
DataParser::_addTouchData(DataParser::Slice &slice, const DataParser::Touch &firstTouch,
                          const DataParser::Touch &lastTouch) {
    long deltaTime = lastTouch.timestamp - firstTouch.timestamp;
    double traveledDistance = Utils::getDistance(firstTouch.x, firstTouch.y, lastTouch.x,
                                                 lastTouch.y);
    double speed = traveledDistance / (double) deltaTime;

    if (speed < 0 || deltaTime <= 0) speed = 0;

    // for Average Speed per Movement Direction and Movement Direction Histogram
    double angle = Utils::getVectorDirection(firstTouch.x, _deviceHeight - firstTouch.y,
                                             lastTouch.x,
                                             _deviceHeight - lastTouch.y);

    if (angle >= 360) angle = 0;

    int directionId = (int) (angle / 45);

    slice.movementDirectionsHistogram.at(directionId) += 1;
    slice.averageSpeedPerMovementDirectionHistogram.at(directionId) += speed;

    // for Travel Distance Histogram
    int distanceId = (int) (traveledDistance / 200);

    slice.travelDistanceHistogram.at(distanceId) += 1;
    slice.averageSpeedRelativeToTravelDistanceHistogram.at(distanceId) += speed;

    // for Extreme Movement Speed Relative to Travel Distance
    slice.extremeSpeedsRelativeToTravelDistanceHistogram.at(distanceId) = std::max(
            slice.extremeSpeedsRelativeToTravelDistanceHistogram.at(distanceId), speed);

    // for Movement Elapsed Time Histogram
    int timeId = (int) (deltaTime / 500);

    timeId = std::clamp(timeId, 0, 8);

    slice.movementElapsedTimeHistogram.at(timeId) += 1;
}

void DataParser::_parseTouchLog(DataParser::Slice &slice, std::vector<std::string> &parsedOutput) {
    slice.lastKeyTimestamp = -1;

    if (parsedOutput.at(2) == "DEVICE_BOUNDARIES") {
        _deviceWidth = StringsHelpers::stringToInt(parsedOutput.at(3));
        _deviceHeight = StringsHelpers::stringToInt(parsedOutput.at(4));

        return;
    }

    float screenPortionWidth = (float) _deviceWidth / 3;
    float screenPortionHeight = (float) _deviceHeight / 3;

    long timestamp = StringsHelpers::stringToLongInt(parsedOutput.at(1));
    int fingerId = StringsHelpers::stringToInt(parsedOutput.at(2));

    int x = StringsHelpers::stringToInt(parsedOutput.at(3));
    int y = StringsHelpers::stringToInt(parsedOutput.at(4));

    int xIndex = (int) ((float) x / screenPortionWidth);
    int yIndex = (int) ((float) y / screenPortionHeight);

    xIndex = std::clamp(xIndex, 0, 2);
    yIndex = std::clamp(yIndex, 0, 2);

    int portionIndex = (yIndex * 3) + xIndex;

    // for Distribution of Actions on the Screen
    slice.distrubtionsOfTouchs.at(portionIndex) += 1;

    if (_firstTouches.find(fingerId) == _firstTouches.end()) {
        _firstTouches.insert({fingerId, {timestamp, x, y}});

        return;
    }

    auto &firstTouch = _firstTouches.at(fingerId);

    if (_lastTouches.find(fingerId) == _lastTouches.end()) {
        if (Utils::getDistance(firstTouch.x, firstTouch.y, x, y) >= 500) {
            _addTouchData(slice, firstTouch, {timestamp, x, y});

            _firstTouches.erase(fingerId);

            return;
        }

        _lastTouches.insert({fingerId, {timestamp, x, y}});

        return;
    }

    auto &lastTouch = _lastTouches.at(fingerId);

    if (timestamp - lastTouch.timestamp > 500) {
        _addTouchData(slice, firstTouch, lastTouch);

        firstTouch = {timestamp, x, y};

        _lastTouches.erase(fingerId);

        return;
    }

    if (Utils::getDistance(firstTouch.x, firstTouch.y, x, y) >= 500) {
        _addTouchData(slice, firstTouch, {timestamp, x, y});

        _firstTouches.erase(fingerId);
        _lastTouches.erase(fingerId);

        return;
    } else {
        lastTouch = {timestamp, x, y};
    }
}

void DataParser::_averageKeystrokeData() {
    for (auto &slice: _slices) {
        int totalKeystrokes = 0;

        for (auto &key: slice->keys) {
            if (key.second->count != 0) {
                key.second->duration /= key.second->count;
                totalKeystrokes += key.second->count;
            }
        }

        if (totalKeystrokes != 0) slice->keyLatency /= totalKeystrokes;
    }
}

void DataParser::_averageTouchData() {
    // check if touch events do have a dangling lastTouch

    for (auto &lastTouch: _lastTouches) {
        _addTouchData(*_slices.back(), lastTouch.second, _firstTouches.at(lastTouch.first));
    }

    _lastTouches.clear();
    _firstTouches.clear();

    for (auto &slice: _slices) {
        for (int i = 0; i < 9; ++i) {
            // average speed per movement direction
            if (i < 8 && slice->movementDirectionsHistogram.at(i) != 0) {
                slice->averageSpeedPerMovementDirectionHistogram.at(
                        i) /= slice->movementDirectionsHistogram.at(i);
            }

            // average speed relative to travel distance
            if (slice->travelDistanceHistogram.at(i) != 0)
                slice->averageSpeedRelativeToTravelDistanceHistogram.at(
                        i) /= slice->travelDistanceHistogram.at(i);
        }
    }
}


void DataParser::combineLogs(const std::string &orientationFilename,
                             const std::string &keyboardFilename,
                             const std::string &touchFilename) {
    FileLogger combinedlogs{"data/" + _name + "_combined.txt"};

    std::map<long, std::string> logValues;

    _loadLogs(logValues, orientationFilename, "ORIENTATION");
    _loadLogs(logValues, keyboardFilename, "KEYBOARD");
    _loadLogs(logValues, touchFilename, "TOUCH");

    for (auto &line : logValues) {
        combinedlogs.addLine(line.second, false);
    }
}

void DataParser::parseDataSlices() {
    std::fstream fileStream;
    std::string lineBuffer;

    fileStream.open("data/" + _name + "_combined.txt");

    if (!fileStream.is_open())
        throw std::runtime_error("File Not Opened");

    _slices.push_back(new Slice());

    while (std::getline(fileStream, lineBuffer)) {
        std::vector<std::string> parsedOutput = StringsHelpers::split(lineBuffer, ' ');

        std::string logName = parsedOutput.at(0);
        long timestamp = StringsHelpers::stringToLongInt(parsedOutput.at(1));

        if (timestamp == 0) continue;

        size_t size = _slices.size();

        for (size_t i = 0; i < size; ++i) {
            auto slice = _slices.at(i);

            if (slice->done)
                continue;

            if (slice->lastTimestamp != -1) {
                long deltaTime = timestamp - slice->lastTimestamp;

                slice->duration += deltaTime;
            }

            slice->lastTimestamp = timestamp;

            if (slice->duration >= 60 * 5 * 1000) // 5 minutes
                slice->done = true;

            if (logName == "KEYBOARD")
                _parseKeyboardLog(*slice, parsedOutput);

            if (logName == "ORIENTATION")
                _parseOrientationLog(*slice, parsedOutput);

            if (logName == "TOUCH")
                _parseTouchLog(*slice, parsedOutput);

            // add a slice every 90 seconds
            if (slice->duration > (90 * 1000) && slice->next) {
                _slices.push_back(new Slice());

                slice->next = false;
            }
        }
    }

    fileStream.close();

    _averageKeystrokeData();
    _averageTouchData();
}

std::vector<DataParser::dataVector> DataParser::getDataVectors() {
    std::vector<dataVector> dataVectors;

    for (auto &slice: _slices) {
        dataVector dataVector;

        // Orientation Histogram
        for (auto &state: slice->states) {
            dataVector.push_back(state.count);
        }

        // Orientation Time Histogram
        for (auto &state: slice->states) {
            dataVector.push_back(state.timeSpent);
        }

        // Keystroke duration
        for (auto &key: slice->keys) {
            dataVector.push_back(key.second->duration);
        }

        // Keystroke latency
        dataVector.push_back(slice->keyLatency);

        // Distribution of Actions on the Screen
        for (auto &screenPortionCount: slice->distrubtionsOfTouchs) {
            dataVector.push_back(screenPortionCount);
        }

        // Movement Direction Histogram
        for (auto &movementDirection: slice->movementDirectionsHistogram) {
            dataVector.push_back(movementDirection);
        }

        // Average Speed per Movement Direction
        for (auto &averageSpeedPerMovementDirection: slice->averageSpeedPerMovementDirectionHistogram) {
            dataVector.push_back(averageSpeedPerMovementDirection);
        }

        // Travel Distance Histogram
        for (auto &travelDistance: slice->travelDistanceHistogram) {
            dataVector.push_back(travelDistance);
        }

        // Extreme Movement Speed Relative to Travel Distance
        for (auto &extremeMovementRelativeToTravelDistance: slice->extremeSpeedsRelativeToTravelDistanceHistogram) {
            dataVector.push_back(extremeMovementRelativeToTravelDistance);
        }

        // Movement Elapsed Time Histogram
        for (auto &movementElapsedTime: slice->movementElapsedTimeHistogram) {
            dataVector.push_back(movementElapsedTime);
        }

        // Average Movement Speed Relative to Travel Distance
        for (auto &averageSpeedRelativeToTravelDistance: slice->averageSpeedRelativeToTravelDistanceHistogram) {
            dataVector.push_back(averageSpeedRelativeToTravelDistance);
        }

        dataVectors.push_back(dataVector);
    }

    return dataVectors;
}
