add_library(ann STATIC
    src/ann.cpp
    src/layer.cpp
    src/shuffled_range.cpp
)

target_include_directories(ann PUBLIC headers)

add_subdirectory(test)
