include(GoogleTest)

add_library(matrix STATIC
    headers/matrix/matrix.h
    src/matrix.cpp
)

target_include_directories(matrix PUBLIC headers)
set_target_properties(matrix PROPERTIES PUBLIC_HEADER "matrix/matrix.h")
target_link_libraries(matrix PUBLIC CONAN_PKG::boost)

add_subdirectory(tests)
