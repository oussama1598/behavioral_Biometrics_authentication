#include "authenticate_route.h"

// hard coded users due to the lack of a database
std::vector<std::string> AuthenticateRoute::_users{"user_1", "user_2"};

void AuthenticateRoute::_sendErrorReponse(httplib::Response &res, const std::string &error) {
    json response = {};

    response["status"] = false;
    response["error"] = error;

    res.set_content(response.dump(), "application/json");
}

float AuthenticateRoute::_authenticate(const std::string &user_id,
                                       const std::vector<double> &sample) {
    std::string unknownUser = user_id == "user_1" ? "user_2" : "user_1";

    DataParser currentUserDataParser{user_id};
    DataParser unknownUserDataParser{unknownUser};

    currentUserDataParser.combineLogs(
            "data/" + user_id + "_orientation_events.txt",
            "data/" + user_id + "_keyboard_events.txt",
            "data/" + user_id + "_touch_events.txt"
    );

    unknownUserDataParser.combineLogs(
            "data/" + unknownUser + "_orientation_events.txt",
            "data/" + unknownUser + "_keyboard_events.txt",
            "data/" + unknownUser + "_touch_events.txt"
    );

    currentUserDataParser.parseDataSlices();
    unknownUserDataParser.parseDataSlices();

    Authenticator authenticator{15, 15, currentUserDataParser, unknownUserDataParser};
    authenticator.train();

    return authenticator.authenticate(sample);
}

void AuthenticateRoute::postRoute(const httplib::Request &req, httplib::Response &res) {
    json body{};
    json response{};

    try {
        body = json::parse(req.body);
    } catch (const std::exception &) {
        _sendErrorReponse(res, "NOT_VALID_BODY");

        return;
    }

    if (!body.contains("user_id") or body["user_id"].empty()) {
        _sendErrorReponse(res, "USER_ID_IS_EMPTY");

        return;
    }


    if (!body.contains("slice_data") or body["slice_data"].empty()) {
        _sendErrorReponse(res, "SLICE_DATA_IS_EMPTY");

        return;
    }

    std::string user_id = body["user_id"];
    std::string slice_data = body["slice_data"];

    if (std::find(_users.begin(), _users.end(), user_id) == _users.end()) {
        _sendErrorReponse(res, "USER_ID_DOES_NOT_EXIST");

        return;
    }

    std::vector<std::string> data = StringsHelpers::split(slice_data, ',');

    std::cout << data.size() << std::endl;

    if (data.size() != 122) {
        _sendErrorReponse(res, "NOT_VALID_SLICE_OF_DATA");

        return;
    }

    std::vector<double> slice;

    for (auto &element: data) {
        try {
            slice.push_back(std::stod(element));
        } catch (std::exception &e) {
            _sendErrorReponse(res, "DATA_SLICE_SHOULD_ONLY_CONTAIN_NUMBERS");

            return;
        }
    }

    response["status"] = true;
    response["authentication"] = _authenticate(user_id, slice) == 1;

    res.set_content(response.dump(), "application/json");
}
