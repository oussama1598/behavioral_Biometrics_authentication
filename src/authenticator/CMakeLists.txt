set(SOURCES authenticator.cpp authenticator.h)

add_library(authenticator SHARED ${SOURCES})
target_link_libraries(authenticator CONAN_PKG::opencv data_parser)

add_executable(authenticator_bin main.cpp)
target_link_libraries(authenticator_bin PRIVATE authenticator)
