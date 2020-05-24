add_library(ann STATIC
    src/ann.cpp
    src/layer.cpp
    src/sync_controller.cpp
    src/utils.h
    src/utils.cpp
    src/shuffled_range.cpp
    src/async_controller.cpp
    src/async_loop.cpp
)

target_include_directories(ann PUBLIC headers)
set_target_properties(ann PROPERTIES PUBLIC_HEADER "ann/iann.h;ann/iann_controller.h")
target_link_libraries(ann PRIVATE matrix)
target_link_libraries(ann PUBLIC minstdb)

add_subdirectory(test)
