#pragma once

#include <nlohmann/json.hpp>
#include "api_server/lib/httplib.h"

using json = nlohmann::json;

class MainRoute {
public:
    static void getRoute(const httplib::Request &, httplib::Response &res);
};

