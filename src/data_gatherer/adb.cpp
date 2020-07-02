#include "adb.h"

// commands

// get touch screen events
// adb shell su 0 getevent -lt /dev/input/event3

// get hardware devices
// adb shell cat /proc/bus/input/devices

void Adb::getDevices() {
    redi::ipstream adbProcess("adb devices", redi::pstreams::pstdout | redi::pstreams::pstderr);

    std::string lineBuffer;

    while (std::getline(adbProcess.out(), lineBuffer)) {
        if (lineBuffer.find("\tdevice") != std::string::npos) {
            std::vector<std::string> parsedData = StringsHelpers::split(lineBuffer, '\t');

            std::cout << parsedData[0] << std::endl;
        }
    }
}

// protocol reference https://www.kernel.org/doc/html/v4.18/input/multi-touch-protocol.html?fbclid=IwAR0BR7xXPjYubsk6em5Hyg2hF_i6cpENp6rMWBmUGboWJjZPknso1PMuqso

void Adb::listenForTouchEvents() {
    std::vector<std::pair<int, int>> touchData;

    redi::ipstream adbProcess("adb shell getevent -lt /dev/input/event3");

    std::string lineBuffer;

    int currentFinderSlotIndex = 0;

    bool waitingForPacket = false;

    while (std::getline(adbProcess.out(), lineBuffer)) {
        std::vector<std::string> parsedData = StringsHelpers::split(
                StringsHelpers::removeMultipleSpaces(lineBuffer),
                ' '
        );

        std::string eventName = parsedData[3];
        std::string eventValue = parsedData[4];

        if (eventName == "SYN_REPORT")
            waitingForPacket = true;

        if (eventName == "ABS_MT_SLOT")
            currentFinderSlotIndex = StringsHelpers::hexStringToInt(eventValue);

        if (eventName == "ABS_MT_TRACKING_ID") {
            if (eventValue != "ffffffff")
                touchData.emplace_back(0, 0);
            else {
                touchData.erase(touchData.begin() + currentFinderSlotIndex);

                if (touchData.size() == 1)
                    currentFinderSlotIndex = 0;
            }
        }

        if (eventName == "ABS_MT_POSITION_X") {
            touchData.at(currentFinderSlotIndex).first = StringsHelpers::hexStringToInt(eventValue);
        }

        if (eventName == "ABS_MT_POSITION_Y") {
            touchData.at(currentFinderSlotIndex).second = StringsHelpers::hexStringToInt(
                    eventValue);

            if (waitingForPacket) {
                std::cout << currentFinderSlotIndex << " "
                          << touchData.at(currentFinderSlotIndex).first << " "
                          << touchData.at(currentFinderSlotIndex).second << std::endl;
            }
        }
    }
}
