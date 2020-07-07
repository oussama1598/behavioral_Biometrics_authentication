#include "adb_events_listener.h"
#include "keyboard_events_listener.h"
#include "orientation_events_listener.h"

void runKeyboardListener() {
    KeyboardEventsListener keyboardEventsListener;
    keyboardEventsListener.listenForEvents();
}

void runAdbListener() {
    AdbEventsListener adbEventsListener;
    adbEventsListener.listenForEvents();
}

void runOrientationListener() {
    OrientationEventsListener orientationEventsListener;
    orientationEventsListener.listenForEvents();
}

int main() {
//    std::thread keyboardListenerThread(runKeyboardListener);
    std::thread adbListenerThread(runAdbListener);
//    std::thread orientationListenerThread(runOrientationListener);

//    keyboardListenerThread.join();
    adbListenerThread.join();
//    orientationListenerThread.join();
}
