include(GoogleTest)

add_library(matrix INTERFACE)

target_include_directories(matrix INTERFACE headers)
set_target_properties(matrix PROPERTIES PUBLIC_HEADER "matrix/matrix.h")
target_link_libraries(matrix INTERFACE CONAN_PKG::boost)

add_subdirectory(tests)
