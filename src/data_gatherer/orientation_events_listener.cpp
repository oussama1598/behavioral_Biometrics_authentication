#include "orientation_events_listener.h"

[[noreturn]] void OrientationEventsListener::listenForEvents() {
    while (true) {
        redi::ipstream eventListener("adb shell 'dumpsys input | grep SurfaceOrientation'");
        std::string outputBuffer;

        while (std::getline(eventListener.out(), outputBuffer)) {
            std::string timestamp = Utils::getCurrentTimeStamp();

            std::string formatedOutput = StringsHelpers::removeMultipleSpaces(outputBuffer);
            std::vector<std::string> parsedOutput = StringsHelpers::split(formatedOutput, ':');
            std::string state = parsedOutput[1].substr(1, 1);

            std::cout << timestamp << " " << state << std::endl;

            break;
        }
    }
}
