#include "main_route.h"

void MainRoute::getRoute(const httplib::Request &, httplib::Response &res) {
    json response = {
            {"API",     "Behavioral Biometrics Authentication"},
            {"VERSION", "1.0.0"}
    };

    res.set_content(response.dump(), "application/json");
}
