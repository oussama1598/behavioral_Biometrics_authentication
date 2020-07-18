#pragma once

#include "lib/httplib.h"
#include "routes/main_route.h"
#include "routes/authenticate_route.h"

class APIServer {
private:
    httplib::Server _server;

public:
    APIServer();

    void listen(const std::string &interface = "0.0.0.0", int port = 8000);
};

