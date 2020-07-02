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
