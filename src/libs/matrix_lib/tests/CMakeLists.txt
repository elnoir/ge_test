find_package(GTest)

add_executable(matrix_test
    matrix_test.cpp
)

target_link_libraries(matrix_test PUBLIC matrix gtest gtest_main)
gtest_discover_tests(matrix_test)
