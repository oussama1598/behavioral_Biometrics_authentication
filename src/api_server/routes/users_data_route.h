#pragma once

#include <nlohmann/json.hpp>
#include "api_server/lib/httplib.h"
#include "helpers/file_logger.h"

using json = nlohmann::json;

class UsersDataRoute {
private:
    static std::vector<std::string> _users;

    static std::vector<std::string> _allowedDataTypes;
private:

    static void _sendErrorResponse(httplib::Response &res, const std::string &error);

public:

    static void postRoute(const httplib::Request &req, httplib::Response &res);
};

