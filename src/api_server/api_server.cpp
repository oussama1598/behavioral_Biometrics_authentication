#include "api_server.h"

APIServer::APIServer() {
    // setup routes
    _server.Get("/", MainRoute::getRoute);
    _server.Get("/authenticate", AuthenticateRoute::getRoute);

    _server.set_error_handler([](const auto &, auto &res) {
        auto fmt = "<p>Error Status: <span style='color:red;'>%d</span></p>";
        char buf[BUFSIZ];

        snprintf(buf, sizeof(buf), fmt, res.status);

        res.set_content(buf, "text/html");
    });
}

void APIServer::listen(const std::string &interface, int port) {
    std::cout << "Listening on interface " << interface << " port " << port << std::endl;

    _server.listen(interface.c_str(), port);
}
