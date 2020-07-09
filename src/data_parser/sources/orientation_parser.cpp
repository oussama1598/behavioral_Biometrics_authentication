#include "orientation_parser.h"

OrientationParser::OrientationParser(const std::string &filename) {
    std::fstream fileStream;
    std::string lineBuffer;

    fileStream.open(filename);

    if (!fileStream.is_open())
        throw std::runtime_error("File Not Opened");

    _slices.push_back({false, 0, -1, {}});

    while (std::getline(fileStream, lineBuffer)) {
        std::vector<std::string> parsedOutput = StringsHelpers::split(lineBuffer, ' ');

        int timestamp = StringsHelpers::stringToInt(parsedOutput[0]);
        int stateId = StringsHelpers::stringToInt(parsedOutput[1]);

        for (auto &slice: _slices) {
            if (slice.done)
                continue;

            if (slice.lastTimestamp != -1) {
                int deltaTime = timestamp - slice.lastTimestamp;

                totalTime += deltaTime;
                slice.duration += deltaTime;

                // State time spent in
                slice.states[stateId].timeSpent += deltaTime;
            }

            slice.lastTimestamp = timestamp;

            if (slice.duration >= 60 * 5) // 5 minutes
                slice.done = true;

            // state event changes
            if (_lastStateId != -1 && _lastStateId != stateId) {
                slice.states[stateId].count += 1;
            }

            _lastStateId = stateId;

            // add a slice every 90 seconds
            if ((size_t) (totalTime / 90) == _slices.size()) {
                _slices.push_back({false, 0, -1, {}});
            }
        }
    }

    std::cout << "ok" << std::endl;
}
