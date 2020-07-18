#include "authenticate_route.h"

void AuthenticateRoute::getRoute(const httplib::Request &req, httplib::Response &res) {
    json response = {};

    if (!req.has_param("user_id")) {
        response["status"] = false;
        response["error"] = "USER_ID_IS_EMPTY";

        res.set_content(response.dump(), "application/json");

        return;
    }
}
