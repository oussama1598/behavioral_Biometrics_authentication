set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

set(SOURCES
        main.cpp

        lib/pstream.h
        lib/httplib.h

        adb_events_listener.cpp
        adb_events_listener.h

        keyboard_events_listener.cpp
        keyboard_events_listener.h
        orientation_events_listener.cpp orientation_events_listener.h)

add_executable(data_gatherer_bin ${SOURCES})
target_link_libraries(data_gatherer_bin PRIVATE helpers Threads::Threads)
