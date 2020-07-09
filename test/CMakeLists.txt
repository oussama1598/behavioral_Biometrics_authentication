add_executable(tests run_tests.cpp data_parser_tests/orientation_tests.cpp data_parser_tests/orientation_tests.h)
target_link_libraries(tests PRIVATE data_parser CONAN_PKG::Catch2)
add_test(NAME tests COMMAND $<TARGET_FILE:tests>)

add_custom_command(
        TARGET tests PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/test/data_parser_tests/test_data/
        ${CMAKE_BINARY_DIR}/bin/test_data/)
