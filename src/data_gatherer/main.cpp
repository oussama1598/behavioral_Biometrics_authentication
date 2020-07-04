#include "adb_events_listener.h"
#include "keyboard_events_listener.h"

int main() {
//    AdbEventsListener adbEventsListener;
//
//    adbEventsListener.listenForEvents();

    KeyboardEventsListener keyboardEventsListener;

    keyboardEventsListener.listenForEvents();
}
