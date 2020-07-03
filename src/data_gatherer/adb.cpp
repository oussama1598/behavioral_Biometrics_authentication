#include "adb.h"

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

void Adb::_listenForTouchEvents(std::string &eventName, std::string &eventValue) {
    if (eventName == "ABS_MT_SLOT")
        _currentFinderSlotIndex = StringsHelpers::hexStringToInt(eventValue);

    if (eventName == "ABS_MT_TRACKING_ID") {
        if (eventValue != "ffffffff")
            _touchEvents.insert({
                                        _currentFinderSlotIndex,
                                        {
                                                0, 0, 0, 0, -1, -1
                                        }
                                });
        else {
            _touchEvents.erase(_currentFinderSlotIndex);

            if (_touchEvents.size() <= 1)
                _currentFinderSlotIndex = 0;
        }
    }

    if (eventName == "ABS_MT_POSITION_X") {
        _touchEvents.at(_currentFinderSlotIndex).x = StringsHelpers::hexStringToInt(eventValue);

        _touchWaitingForPacket = true;
    }

    if (eventName == "ABS_MT_POSITION_Y") {
        _touchEvents.at(_currentFinderSlotIndex).y = StringsHelpers::hexStringToInt(
                eventValue);
    }

    if (eventName == "ABS_MT_TOUCH_MAJOR")
        _touchEvents.at(_currentFinderSlotIndex).touchMajor = StringsHelpers::hexStringToInt(
                eventValue);

    if (eventName == "ABS_MT_WIDTH_MAJOR") {
        _touchEvents.at(_currentFinderSlotIndex).widthMajor = StringsHelpers::hexStringToInt(
                eventValue);

        if (_touchWaitingForPacket) {
            std::cout << _currentFinderSlotIndex << " "
                      << _touchEvents.at(_currentFinderSlotIndex).x << " "
                      << _touchEvents.at(_currentFinderSlotIndex).y << " "
                      << _touchEvents.at(_currentFinderSlotIndex).touchMajor /
                         _touchEvents.at(_currentFinderSlotIndex).widthMajor
                      << std::endl;

            _touchWaitingForPacket = false;
        }
    }
}

// protocol reference https://www.kernel.org/doc/Documentation/input/event-codes.txt

void Adb::_listenForAccelerometerEvents(std::string &eventName, std::string &eventValue) {
    if (eventName == "REL_X") {
        _accelerometerWaitingForPacket = true;

        _accelerometerEvent.x = StringsHelpers::hexStringToInt(eventValue);
    }

    if (eventName == "REL_Y") {
        _accelerometerEvent.y = StringsHelpers::hexStringToInt(eventValue);
    }

    if (eventName == "REL_Z") {
        _accelerometerEvent.z = StringsHelpers::hexStringToInt(eventValue);

        if (_accelerometerWaitingForPacket) {
            std::cout << _accelerometerEvent.x << " "
                      << _accelerometerEvent.y << " "
                      << _accelerometerEvent.z
                      << std::endl;

            _accelerometerWaitingForPacket = false;
        }
    }
}

void Adb::listenForGyroscopeEvents() {
    redi::ipstream adbProcess("adb shell getevent -lt /dev/input/event5");

    std::string lineBuffer;

    GyroscopeEvent event{0, -1, -1, -1};
    bool waitingForPacket = false;

    while (std::getline(adbProcess.out(), lineBuffer)) {
        std::vector<std::string> parsedData = StringsHelpers::split(
                StringsHelpers::removeMultipleSpaces(lineBuffer),
                ' '
        );

        std::string eventName = parsedData[3];
        std::string eventValue = parsedData[4];

        if (eventName == "REL_RX") {
            waitingForPacket = true;

            event.x = StringsHelpers::hexStringToInt(eventValue);
        }

        if (eventName == "REL_RY") {
            event.y = StringsHelpers::hexStringToInt(eventValue);
        }

        if (eventName == "REL_RZ") {
            event.z = StringsHelpers::hexStringToInt(eventValue);

            if (waitingForPacket) {
                std::cout << event.x << " "
                          << event.y << " "
                          << event.z
                          << std::endl;

                waitingForPacket = false;
            }
        }
    }
}

void Adb::listenForButtonsEvents() {
    redi::ipstream adbProcess("adb shell getevent -lt");

    std::string lineBuffer;

    while (std::getline(adbProcess.out(), lineBuffer)) {
        std::cout <<
                  StringsHelpers::removeMultipleSpaces(lineBuffer) << std::endl;

    }
}

void Adb::listenForEvents() {
    redi::ipstream eventListener("adb shell getevent -lt");
    std::string outputBuffer;

    while (std::getline(eventListener.out(), outputBuffer)) {
        std::string formatedOutput = StringsHelpers::removeMultipleSpaces(outputBuffer);

        if (outputBuffer.find_first_of('[') != 0) {
            continue;
        }

        std::vector<std::string> parsedOutput = StringsHelpers::split(formatedOutput, ' ');

        std::string eventTypePath = parsedOutput[2];
        std::string eventName = parsedOutput[4];
        std::string eventValue = parsedOutput[5];

        std::string eventType = StringsHelpers::split(eventTypePath, '/')[3];

        eventType = eventType.substr(0, eventType.size() - 1);

        if (_eventsList.find(eventType) != _eventsList.end()) {
            _eventsList.at(eventType)(eventName, eventValue);
        }
    }
}
