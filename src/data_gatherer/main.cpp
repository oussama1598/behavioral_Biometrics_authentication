#include "adb_events_listener.h"
#include "keyboard_events_listener.h"

void runKeyboardListener() {
    KeyboardEventsListener keyboardEventsListener;
    keyboardEventsListener.listenForEvents();
}

void runAdbListener() {
    AdbEventsListener adbEventsListener;
    adbEventsListener.listenForEvents();
}

int main() {
    std::thread keyboardListenerThread(runKeyboardListener);
    std::thread adbListenerThread(runAdbListener);

    keyboardListenerThread.join();
    adbListenerThread.join();
}
