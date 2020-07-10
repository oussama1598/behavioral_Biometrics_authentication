#include "adb_events_listener.h"

// protocol reference touch screen https://www.kernel.org/doc/html/v4.18/input/multi-touch-protocol.html?fbclid=IwAR0BR7xXPjYubsk6em5Hyg2hF_i6cpENp6rMWBmUGboWJjZPknso1PMuqso
// protocol reference other events https://www.kernel.org/doc/Documentation/input/event-codes.txt

void AdbEventsListener::_getDeviceBoundaries() {
    redi::ipstream widthEvent("adb shell \"dumpsys input | grep 'X: min='\"");
    redi::ipstream heightEvent("adb shell \"dumpsys input | grep 'Y: min='\"");

    std::string outputBuffer;

    std::string width, height;

    while (std::getline(widthEvent, outputBuffer)) {
        std::string formatedOutput = StringsHelpers::removeMultipleSpaces(outputBuffer);
        std::vector<std::string> parsedOutput = StringsHelpers::split(formatedOutput, ',');
        std::vector<std::string> parsedWidth = StringsHelpers::split(parsedOutput[1], '=');

        width = parsedWidth[1];
    }

    while (std::getline(heightEvent, outputBuffer)) {
        std::string formatedOutput = StringsHelpers::removeMultipleSpaces(outputBuffer);
        std::vector<std::string> parsedOutput = StringsHelpers::split(formatedOutput, ',');
        std::vector<std::string> parsedData = StringsHelpers::split(parsedOutput[1], '=');

        height = parsedData[1];
    }


    std::stringstream ss;

    ss << Utils::getCurrentTimeStamp() << " "
       << "DEVICE_BOUNDARIES" << " "
       << width << " "
       << height;

    _touchEventsFileLogger.addLine(ss.str());
}

void AdbEventsListener::_extractTouchEvents(std::string &eventName, std::string &eventValue) {
    if (eventName == "ABS_MT_SLOT")
        _currentFinderSlotIndex = StringsHelpers::hexStringToInt(eventValue);

    if (eventName == "ABS_MT_TRACKING_ID") {
        if (eventValue != "ffffffff")
            _touchEvents.insert({
                                        _currentFinderSlotIndex,
                                        {
                                                "0", 0, 0, 0, -1, -1
                                        }
                                });
        else {
            _touchEvents.erase(_currentFinderSlotIndex);

            if (_touchEvents.size() <= 1)
                _currentFinderSlotIndex = 0;
        }
    }

    if (eventName == "ABS_MT_POSITION_X") {
        std::string timestamp = Utils::getCurrentTimeStamp();

        _touchEvents.at(_currentFinderSlotIndex).id = _currentFinderSlotIndex;
        _touchEvents.at(_currentFinderSlotIndex).timestamp = timestamp;
        _touchEvents.at(_currentFinderSlotIndex).x = StringsHelpers::hexStringToInt(eventValue);

        _touchWaitingForPacket = true;
    }

    if (eventName == "ABS_MT_POSITION_Y") {
        _touchEvents.at(_currentFinderSlotIndex).y = StringsHelpers::hexStringToInt(
                eventValue);


        if (_touchWaitingForPacket) {
            std::stringstream output;

            output << _touchEvents.at(_currentFinderSlotIndex).timestamp << " "
                   << _touchEvents.at(_currentFinderSlotIndex).id << " "
                   << _touchEvents.at(_currentFinderSlotIndex).x << " "
                   << _touchEvents.at(_currentFinderSlotIndex).y << " "
                   << _touchEvents.at(_currentFinderSlotIndex).pressure;

            _touchEventsFileLogger.addLine(output.str());

            _touchWaitingForPacket = false;
        }
    }

    if (eventName == "ABS_MT_TOUCH_MAJOR") {
        if (_touchEvents.find(_currentFinderSlotIndex) == _touchEvents.end())
            return;

        _touchEvents.at(_currentFinderSlotIndex).touchMajor = StringsHelpers::hexStringToInt(
                eventValue);
    }

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
        std::string timestamp = Utils::getCurrentTimeStamp();

        _accelerometerWaitingForPacket = true;

        _accelerometerEvent.x = StringsHelpers::hexStringToInt(eventValue);
        _accelerometerEvent.timestamp = timestamp;
    }

    if (eventName == "REL_Y") {
        _accelerometerEvent.y = StringsHelpers::hexStringToInt(eventValue);
    }

    if (eventName == "REL_Z") {
        _accelerometerEvent.z = StringsHelpers::hexStringToInt(eventValue);

        if (_accelerometerWaitingForPacket) {
            std::cout << _accelerometerEvent.timestamp << " "
                      << _accelerometerEvent.x << " "
                      << _accelerometerEvent.y << " "
                      << _accelerometerEvent.z
                      << std::endl;

            _accelerometerWaitingForPacket = false;
        }
    }
}

void AdbEventsListener::_extractGyroscopeEvents(std::string &eventName, std::string &eventValue) {
    if (eventName == "REL_RX") {
        std::string timestamp = Utils::getCurrentTimeStamp();

        _gyroscopeWaitingForPacket = true;

        _gyroscopeEvent.x = StringsHelpers::hexStringToInt(eventValue);
        _gyroscopeEvent.timestamp = timestamp;
    }

    if (eventName == "REL_RY") {
        _gyroscopeEvent.y = StringsHelpers::hexStringToInt(eventValue);
    }

    if (eventName == "REL_RZ") {
        _gyroscopeEvent.z = StringsHelpers::hexStringToInt(eventValue);

        if (_gyroscopeWaitingForPacket) {
            std::cout << "Gyro" << " "
                      << _gyroscopeEvent.timestamp << " "
                      << _gyroscopeEvent.x << " "
                      << _gyroscopeEvent.y << " "
                      << _gyroscopeEvent.z
                      << std::endl;

            _gyroscopeWaitingForPacket = false;
        }
    }
}

void AdbEventsListener::_extractButtonsEvents(std::string &eventName, std::string &eventValue) {
    if (_buttonsIDs.find(eventName) != _buttonsIDs.end()) {
        std::string timestamp = Utils::getCurrentTimeStamp();
        std::stringstream output;

        int buttonId = _buttonsIDs.at(eventName);
        bool pressed = eventValue == "DOWN";

        output << timestamp << " "
               << eventName << " "
               << buttonId << " "
               << pressed;

        _buttonEventsFileLogger.addLine(output.str());
    }
}

void AdbEventsListener::listenForEvents() {
    _getDeviceBoundaries();

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
