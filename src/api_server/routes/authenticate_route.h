#pragma once

#include <nlohmann/json.hpp>

#include "api_server/lib/httplib.h"
#include "helpers/strings_helpers.h"
#include "authenticator/authenticator.h"

using json = nlohmann::json;

class AuthenticateRoute {
private:
    static std::vector<std::string> _users;

private:
    static void _sendErrorReponse(httplib::Response &res, const std::string &error);

public:

    static float
    _authenticate(const std::string &user_id, const std::vector<double> &sample);

    static void postRoute(const httplib::Request &req, httplib::Response &res);
};

