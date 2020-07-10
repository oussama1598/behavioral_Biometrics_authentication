#pragma once

#include "lib/httplib.h"
#include "helpers/file_logger.h"
#include "helpers/utils.h"

class KeyboardEventsListener {
private:
    httplib::Server _server;

    static FileLogger _keyboardEventsFileLogger;

private:
    static void _mainRoute(const httplib::Request &req, httplib::Response &res);

public:
    KeyboardEventsListener();

    void listenForEvents();
};

