#include "keyboard_parser.h"

KeyboardParser::KeyboardParser(const std::string &filename) {
    std::fstream fileStream;
    std::string lineBuffer;

    fileStream.open(filename);

    if (!fileStream.is_open())
        throw std::runtime_error("File Not Opened");

    _slices.push_back({false, 0, -1});

    while (std::getline(fileStream, lineBuffer)) {
        std::vector<std::string> parsedOutput = StringsHelpers::split(lineBuffer, ' ');

        long timestamp = StringsHelpers::stringToLongInt(parsedOutput[0]);
        int keyCode = StringsHelpers::stringToInt(parsedOutput[2]);
        std::string keyAction = parsedOutput[1];

        for (auto &slice: _slices) {
            if (slice.done)
                continue;

            if (slice.lastTimestamp != -1) {
                int deltaTime = timestamp - slice.lastTimestamp;

                _totalTime += deltaTime;
                slice.duration += deltaTime;
            }

            slice.lastTimestamp = timestamp;

            if (slice.duration >= 60 * 5) // 5 minutes
                slice.done = true;

            // key events
            if (keyAction == "press") {
                slice.keys[keyCode].pressedTimestamp = timestamp;
                slice.keys[keyCode].count += 1;
            }

            if (keyAction == "release") {
                int deltaTime = timestamp - slice.keys[keyCode].pressedTimestamp;

                slice.keys[keyCode].duration += deltaTime;
            }
        }
    }

    std::cout << "ok" << std::endl;
}
