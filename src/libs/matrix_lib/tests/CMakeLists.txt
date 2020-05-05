find_package(GTest REQUIRED)

add_executable(matrix_test
    matrix_test.cpp
)

target_link_libraries(matrix_test PRIVATE GTest::GTest GTest::Main matrix)
gtest_discover_tests(matrix_test)
