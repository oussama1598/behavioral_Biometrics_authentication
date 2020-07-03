#pragma once

#include <iostream>
#include <string>
#include <sstream>
#include <map>
#include <functional>

#include "lib/pstream.h"
#include "helpers/strings_helpers.h"

class Adb {
private:
    struct TouchEvent {
        int timestamp;
        int x, y;
        int pressure;
        int touchMajor = -1, widthMajor = -1;
    };

    struct AccelerometerEvent {
        int timestamp;
        int x, y, z;
    };

    struct GyroscopeEvent {
        int timestamp;
        int x, y, z;
    };

    struct ButtonEvent {
        int ButtonId;
        bool pressed;
    };

    std::map<std::string, std::function<void(std::string &, std::string &)>> _eventsList
            {
                    {"event3", [=](std::string &eventName, std::string &eventValue) {
                        this->_listenForTouchEvents(eventName, eventValue);
                    }},
                    {"event4", [=](std::string &eventName, std::string &eventValue) {
                        this->_listenForAccelerometerEvents(eventName, eventValue);
                    }}
            };

    // for touch events
    std::map<int, TouchEvent> _touchEvents;
    int _currentFinderSlotIndex{0};
    bool _touchWaitingForPacket{false};

    // for accelerometer events
    AccelerometerEvent _accelerometerEvent{0, -1, -1, -1};
    bool _accelerometerWaitingForPacket{false};

private:

    void _listenForTouchEvents(std::string &eventName, std::string &eventValue);

    void _listenForAccelerometerEvents(std::string &eventName, std::string &eventValue);

public:

    void getDevices();

    void listenForGyroscopeEvents();

    void listenForButtonsEvents();

    void listenForEvents();
};

