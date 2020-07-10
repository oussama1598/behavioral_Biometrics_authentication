set(SOURCES strings_helpers.h strings_helpers.cpp utils.cpp utils.h file_logger.cpp file_logger.h)

add_library(helpers ${SOURCES})
target_include_directories(helpers INTERFACE ..)
