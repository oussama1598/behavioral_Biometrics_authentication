set(SOURCES strings_helpers.h strings_helpers.cpp)

add_library(helpers ${SOURCES})
target_include_directories(helpers INTERFACE ..)
