#include "keyboard_events_listener.h"

KeyboardEventsListener::KeyboardEventsListener() {
    _server.Get("/", _mainRoute);
}

void KeyboardEventsListener::_mainRoute(const httplib::Request &req, httplib::Response &res) {
    if (req.has_param("keycode")) {
        std::string keyCode = req.get_param_value("keycode");

        std::cout << "Keyboard Event" << " " << keyCode << std::endl;
    }

    res.set_content("Ok", "text/plain");
}

void KeyboardEventsListener::listenForEvents() {
    _server.listen("0.0.0.0", 8000);
}
