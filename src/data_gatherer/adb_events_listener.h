#pragma once

#include <iostream>
#include <string>
#include <sstream>
#include <map>
#include <functional>

#include "lib/pstream.h"
#include "helpers/strings_helpers.h"
#include "helpers/utils.h"
#include "file_logger.h"

class AdbEventsListener {
private:
    struct TouchEvent {
        std::string timestamp;
        int id;
        int x, y;
        int pressure;
        int touchMajor = -1, widthMajor = -1;
    };

    struct AccelerometerEvent {
        std::string timestamp;
        int x, y, z;
    };

    struct GyroscopeEvent {
        std::string timestamp;
        int x, y, z;
    };

    std::map<std::string, std::function<void(std::string &, std::string &)>> _eventsList
            {
                    {"event6", [=](std::string &eventName, std::string &eventValue) {
                        this->_extractTouchEvents(eventName, eventValue);
                        this->_extractButtonsEvents(eventName, eventValue);
                    }},
                    {
                     "event1", [=](std::string &eventName, std::string &eventValue) {
                        this->_extractButtonsEvents(eventName, eventValue);
                    }}
            };

    // for touch events
    std::map<int, TouchEvent> _touchEvents;
    int _currentFinderSlotIndex{0};
    bool _touchWaitingForPacket{false};

    // for accelerometer events
    AccelerometerEvent _accelerometerEvent{"0", -1, -1, -1};
    bool _accelerometerWaitingForPacket{false};

    // for gyroscope events
    GyroscopeEvent _gyroscopeEvent{"0", -1, -1, -1};
    bool _gyroscopeWaitingForPacket = false;

    // for buttons events
    std::map<std::string, int> _buttonsIDs{
            {"KEY_HOME",       0},
            {"KEY_BACK",       1},
            {"KEY_APPSELECT",  2},
            {"KEY_VOLUMEUP",   3},
            {"KEY_VOLUMEDOWN", 4},
            {"KEY_POWER",      5}
    };

    // fileLoggers
    FileLogger _touchEventsFileLogger{"data/" + Utils::getCurrentTime() + "_touch_events.txt"};
    FileLogger _buttonEventsFileLogger{"data/" + Utils::getCurrentTime() + "_button_events.txt"};

private:

    void _extractTouchEvents(std::string &eventName, std::string &eventValue);

    void _extractAccelerometerEvents(std::string &eventName, std::string &eventValue);

    void _extractGyroscopeEvents(std::string &eventName, std::string &eventValue);

    void _extractButtonsEvents(std::string &eventName, std::string &eventValue);

public:
    void listenForEvents();
};

