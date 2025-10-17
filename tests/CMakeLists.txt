cmake_minimum_required( VERSION 3.26 )

configure_file (
    "${CMAKE_CURRENT_SOURCE_DIR}/include/Paths.hpp.in"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/Paths.hpp"
)

make_executable ( ${TEST_TARGET_NAME} DEPS ${LIB_TARGET_NAME} catch2 )

add_test (
    NAME ${TEST_TARGET_NAME}
    COMMAND ${TEST_TARGET_NAME}
)
