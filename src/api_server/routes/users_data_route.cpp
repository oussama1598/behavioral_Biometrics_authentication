#include "users_data_route.h"

std::vector<std::string> UsersDataRoute::_users{"user_1", "user_2"};
std::vector<std::string> UsersDataRoute::_allowedDataTypes{"orientation", "keyboard", "touch"};

void UsersDataRoute::_sendErrorResponse(httplib::Response &res, const std::string &error) {
    json response = {};

    response["status"] = false;
    response["error"] = error;

    res.set_content(response.dump(), "application/json");
}

void UsersDataRoute::postRoute(const httplib::Request &req, httplib::Response &res) {
    json body{};
    json response{};

    try {
        body = json::parse(req.body);
    } catch (const std::exception &) {
        _sendErrorResponse(res, "NOT_VALID_BODY");

        return;
    }

    if (!body.contains("user_id") or body["user_id"] == "") {
        _sendErrorResponse(res, "USER_ID_IS_EMPTY");

        return;
    }

    if (!body.contains("data_type") or body["data_type"] == "") {
        _sendErrorResponse(res, "DATA_TYPE_IS_EMPTY");

        return;
    }

    if (!body.contains("raw_data") or body["raw_data"] == "") {
        _sendErrorResponse(res, "RAW_DATA_IS_EMPTY");

        return;
    }

    std::string user_id = body["user_id"];
    std::string data_type = body["data_type"];
    std::string raw_data = body["raw_data"];

    if (std::find(_users.begin(), _users.end(), user_id) == _users.end()) {
        _sendErrorResponse(res, "USER_ID_DOES_NOT_EXIST");

        return;
    }

    if (std::find(_allowedDataTypes.begin(), _allowedDataTypes.end(), data_type) ==
        _allowedDataTypes.end()) {
        _sendErrorResponse(res, "DATA_TYPE_IS_NOT_ALLOWED");

        return;
    }

    FileLogger fileLogger{"data/" + user_id + "_" + data_type + "_events.txt"};

    fileLogger.addLine(raw_data, true);

    std::cout << "data/" + user_id + "_" + data_type + "_events.txt" << " " << raw_data
              << std::endl;

    response["status"] = true;
    response["message"] = "Data was added successfly";

    res.set_content(response.dump(), "application/json");
}
