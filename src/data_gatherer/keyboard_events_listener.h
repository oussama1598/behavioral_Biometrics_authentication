#pragma once

#include "lib/httplib.h"

class KeyboardEventsListener {
private:
    httplib::Server _server;

private:
    static void _mainRoute(const httplib::Request &req, httplib::Response &res);

public:
    KeyboardEventsListener();

    void listenForEvents();
};

