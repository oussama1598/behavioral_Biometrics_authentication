set(SOURCES main.cpp lib/httplib.h api_server.cpp api_server.h routes/main_route.cpp routes/main_route.h routes/authenticate_route.cpp routes/authenticate_route.h)

add_executable(api_server_bin ${SOURCES})
target_link_libraries(api_server_bin PRIVATE CONAN_PKG::nlohmann_json helpers authenticator)

add_custom_command(
        TARGET api_server_bin PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/data/
        ${CMAKE_BINARY_DIR}/bin/data/)
