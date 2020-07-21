#include "api_server.h"

int main() {
    APIServer apiServer;

    apiServer.listen("0.0.0.0", std::stoi(std::getenv("PORT")));
}
