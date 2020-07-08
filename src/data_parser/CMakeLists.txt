set(SOURCES main.cpp sources/orientation_parser.cpp sources/orientation_parser.h data_parser.cpp data_parser.h)

add_executable(data_parser ${SOURCES})
target_link_libraries(data_parser PRIVATE helpers)
