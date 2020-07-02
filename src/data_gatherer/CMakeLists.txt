set(SOURCES main.cpp lib/pstream.h adb.cpp adb.h)

add_executable(data_gatherer ${SOURCES})
target_link_libraries(data_gatherer helpers)
