#include "authenticate_route.h"

void AuthenticateRoute::getRoute(const httplib::Request &req, httplib::Response &res) {
    json response = {};

    if (!req.has_param("user_id") or req.get_param_value("user_id").empty()) {
        response["status"] = false;
        response["error"] = "USER_ID_IS_EMPTY";

        res.set_content(response.dump(), "application/json");

        return;
    }


    if (!req.has_param("slice_data") or req.get_param_value("slice_data").empty()) {
        response["status"] = false;
        response["error"] = "SLICE_DATA_IS_EMPTY";

        res.set_content(response.dump(), "application/json");

        return;
    }

    std::string user_id = req.get_param_value("user_id");
    std::string slice_data = req.get_param_value("slice_data");

    response["status"] = true;
    response["authentication"] = true;

    res.set_content(response.dump(), "application/json");
}
