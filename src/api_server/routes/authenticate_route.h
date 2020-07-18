#pragma once

#include <nlohmann/json.hpp>
#include "api_server/lib/httplib.h"

using json = nlohmann::json;

class AuthenticateRoute {
public:
    static void getRoute(const httplib::Request &req, httplib::Response &res);
};

