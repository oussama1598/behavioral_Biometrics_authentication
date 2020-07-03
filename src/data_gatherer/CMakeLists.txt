set(SOURCES main.cpp lib/pstream.h adb_events_listener.cpp adb_events_listener.h)

add_executable(data_gatherer ${SOURCES})
target_link_libraries(data_gatherer helpers)
