set(SOURCES main.cpp lib/matplotlibcpp.h data_visualizer.cpp data_visualizer.h)

find_package(PythonLibs REQUIRED)
include_directories(${PYTHON_INCLUDE_DIRS})

add_executable(data_visualizer_bin ${SOURCES})
target_link_libraries(data_visualizer_bin PRIVATE ${PYTHON_LIBRARIES} data_parser)
