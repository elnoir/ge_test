find_package(GTest REQUIRED)

add_executable(minstdb_test
    db_test.h
    db_test.cpp
)

target_link_libraries(minstdb_test PRIVATE GTest::GTest GTest::Main minstdb)
