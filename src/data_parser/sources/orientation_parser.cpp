#include "orientation_parser.h"

OrientationParser::OrientationParser(const std::string &filename) {
    std::fstream fileStream;
    std::string lineBuffer;

    fileStream.open(filename);

    if (!fileStream.is_open())
        throw std::runtime_error("File Not Opened");

    _slices.push_back({0, -1, {}});

    while (std::getline(fileStream, lineBuffer)) {
        std::vector<std::string> parsedOutput = StringsHelpers::split(lineBuffer, ' ');

        int timestamp = StringsHelpers::stringToInt(parsedOutput[0]);
        int stateId = StringsHelpers::stringToInt(parsedOutput[1]);

        Slice &currentSlice = _slices[_currentSliceIndex];

        if (currentSlice.lastTimestamp != -1) {
            currentSlice.duration += timestamp - currentSlice.lastTimestamp;
        }

        _slices[_currentSliceIndex].lastTimestamp = timestamp;

        std::cout << currentSlice.duration << " "
                  << stateId << std::endl;
    }
}
