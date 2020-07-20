set(SOURCES main.cpp lib/httplib.h api_server.cpp api_server.h routes/main_route.cpp routes/main_route.h routes/authenticate_route.cpp routes/authenticate_route.h)

add_executable(api_server_bin ${SOURCES})
target_link_libraries(api_server_bin PRIVATE CONAN_PKG::nlohmann_json helpers authenticator)
