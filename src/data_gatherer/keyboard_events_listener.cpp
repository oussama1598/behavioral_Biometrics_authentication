#include "keyboard_events_listener.h"

FileLogger KeyboardEventsListener::_keyboardEventsFileLogger{
        "data/" + Utils::getCurrentTime() + "_keyboard_events.txt"};

KeyboardEventsListener::KeyboardEventsListener() {
    _server.Get("/", _mainRoute);
}

void KeyboardEventsListener::_mainRoute(const httplib::Request &req, httplib::Response &res) {
    if (req.has_param("keycode")) {
        std::string keyCode = req.get_param_value("keycode");
        std::string timestamp = Utils::getCurrentTimeStamp();

        std::stringstream output;

        output << timestamp << " " << keyCode;

        std::string strOutput = output.str();

        std::cout << strOutput << std::endl;

        _keyboardEventsFileLogger.addLine(strOutput);
    }

    res.set_content("Ok", "text/plain");
}

void KeyboardEventsListener::listenForEvents() {
    _server.listen("0.0.0.0", 8000);
}
