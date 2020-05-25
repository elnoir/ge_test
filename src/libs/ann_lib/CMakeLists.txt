add_library(ann STATIC
    src/ann.cpp
    src/ann.h
    src/async_command_defs.h
    src/async_command_queue.h
    src/async_controller.cpp
    src/async_controller.h
    src/async_loop.cpp
    src/async_loop.h
    src/layer.cpp
    src/layer.h
    src/shuffled_range.cpp
    src/shuffled_range.h
    src/sync_controller.cpp
    src/sync_controller.h
    src/utils.cpp
    src/utils.h
)

target_include_directories(ann PUBLIC headers)
set_target_properties(ann PROPERTIES PUBLIC_HEADER "ann/iann.h;ann/iann_controller.h;ann/async_command.h")
target_link_libraries(ann PRIVATE matrix)
target_link_libraries(ann PUBLIC minstdb)

add_subdirectory(test)