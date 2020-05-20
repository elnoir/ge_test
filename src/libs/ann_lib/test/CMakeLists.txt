find_package(GTest REQUIRED)

add_executable(ann_test
    shuffle_test.cpp
)

target_link_libraries(ann_test PRIVATE GTest::GTest GTest::Main ann CONAN_PKG::boost)
