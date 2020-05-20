add_library(ann STATIC
    src/ann.cpp
    src/layer.cpp
)

target_include_directories(ann PUBLIC headers)
set_target_properties(ann PROPERTIES PUBLIC_HEADER "ann/iann.h")
target_link_libraries(ann PUBLIC matrix)