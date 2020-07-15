set(SOURCES data_parser.cpp data_parser.h)

add_library(data_parser SHARED ${SOURCES})
target_link_libraries(data_parser helpers)
target_include_directories(helpers INTERFACE ..)

add_executable(data_parser_bin main.cpp)
target_link_libraries(data_parser_bin PRIVATE data_parser)
