cmake_minimum_required(VERSION 3.10)
project(ge_ai_app LANGUAGES CXX)

if(EXISTS ${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
    include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
    conan_basic_setup(TARGET)
endif()

add_subdirectory(src/libs)
add_subdirectory(src/apps)
