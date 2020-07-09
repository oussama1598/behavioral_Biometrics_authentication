set(SOURCES sources/orientation_parser.cpp sources/orientation_parser.h data_parser.cpp data_parser.h sources/keyboard_parser.cpp sources/keyboard_parser.h)

add_library(data_parser ${SOURCES})
target_link_libraries(data_parser helpers)
target_include_directories(helpers INTERFACE ..)

add_executable(data_parser_bin main.cpp)
target_link_libraries(data_parser_bin PRIVATE data_parser)
