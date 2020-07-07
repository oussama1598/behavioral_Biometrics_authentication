#include "adb_events_listener.h"

// protocol reference touch screen https://www.kernel.org/doc/html/v4.18/input/multi-touch-protocol.html?fbclid=IwAR0BR7xXPjYubsk6em5Hyg2hF_i6cpENp6rMWBmUGboWJjZPknso1PMuqso
// protocol reference other events https://www.kernel.org/doc/Documentation/input/event-codes.txt

void AdbEventsListener::_extractTouchEvents(std::string &eventName, std::string &eventValue) {
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
        _touchEvents.at(_currentFinderSlotIndex).id = _currentFinderSlotIndex;

        _touchWaitingForPacket = true;
    }

    if (eventName == "ABS_MT_POSITION_Y") {
        _touchEvents.at(_currentFinderSlotIndex).y = StringsHelpers::hexStringToInt(
                eventValue);

        if (_touchWaitingForPacket) {
            std::cout << _touchEvents.at(_currentFinderSlotIndex).id << " "
                      << _touchEvents.at(_currentFinderSlotIndex).x << " "
                      << _touchEvents.at(_currentFinderSlotIndex).y << " "
                      << _touchEvents.at(_currentFinderSlotIndex).pressure
                      << std::endl;

            _touchWaitingForPacket = false;
        }
    }

    if (eventName == "ABS_MT_TOUCH_MAJOR")
        _touchEvents.at(_currentFinderSlotIndex).touchMajor = StringsHelpers::hexStringToInt(
                eventValue);

    if (eventName == "ABS_MT_WIDTH_MAJOR") {
        _touchEvents.at(_currentFinderSlotIndex).widthMajor = StringsHelpers::hexStringToInt(
                eventValue);

        _touchEvents.at(_currentFinderSlotIndex).pressure =
                _touchEvents.at(_currentFinderSlotIndex).touchMajor /
                _touchEvents.at(_currentFinderSlotIndex).widthMajor;
    }
}

void
AdbEventsListener::_extractAccelerometerEvents(std::string &eventName, std::string &eventValue) {
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

void AdbEventsListener::_extractGyroscopeEvents(std::string &eventName, std::string &eventValue) {
    if (eventName == "REL_RX") {
        _gyroscopeWaitingForPacket = true;

        _gyroscopeEvent.x = StringsHelpers::hexStringToInt(eventValue);
    }

    if (eventName == "REL_RY") {
        _gyroscopeEvent.y = StringsHelpers::hexStringToInt(eventValue);
    }

    if (eventName == "REL_RZ") {
        _gyroscopeEvent.z = StringsHelpers::hexStringToInt(eventValue);

        if (_gyroscopeWaitingForPacket) {
            std::cout << "Gyro" << " " << _gyroscopeEvent.x << " "
                      << _gyroscopeEvent.y << " "
                      << _gyroscopeEvent.z
                      << std::endl;

            _gyroscopeWaitingForPacket = false;
        }
    }
}

void AdbEventsListener::_extractButtonsEvents(std::string &eventName, std::string &eventValue) {
    if (_buttonsIDs.find(eventName) != _buttonsIDs.end()) {
        ButtonEvent event{_buttonsIDs.at(eventName), eventValue == "DOWN"};

        std::cout << eventName << " " << event.ButtonId << " " << event.pressed << std::endl;
    }
}

void AdbEventsListener::getDevices() {
    redi::ipstream adbProcess("adb devices", redi::pstreams::pstdout | redi::pstreams::pstderr);

    std::string lineBuffer;

    while (std::getline(adbProcess.out(), lineBuffer)) {
        if (lineBuffer.find("\tdevice") != std::string::npos) {
            std::vector<std::string> parsedData = StringsHelpers::split(lineBuffer, '\t');

            std::cout << parsedData[0] << std::endl;
        }
    }
}

void AdbEventsListener::listenForEvents() {
    redi::ipstream eventListener("adb shell getevent -lt");
    std::string outputBuffer;

    while (std::getline(eventListener.out(), outputBuffer)) {
        std::string formatedOutput = StringsHelpers::removeMultipleSpaces(outputBuffer);

        if (outputBuffer.find_first_of('[') != 0) {
            continue;
        }

        std::vector<std::string> parsedOutput = StringsHelpers::split(formatedOutput, ' ');

        std::string eventTypePath = parsedOutput[1];
        std::string eventName = parsedOutput[3];
        std::string eventValue = parsedOutput[4];

        std::string eventType = StringsHelpers::split(eventTypePath, '/')[3];

        eventType = eventType.substr(0, eventType.size() - 1);

        if (_eventsList.find(eventType) != _eventsList.end()) {
            _eventsList.at(eventType)(eventName, eventValue);
        }
    }
}
