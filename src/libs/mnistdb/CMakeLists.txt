add_library(minstdb STATIC
    headers/db/idb.h
    src/db.h
    src/db.cpp
)

target_include_directories(minstdb PUBLIC headers)
set_target_properties(minstdb PROPERTIES PUBLIC_HEADER "db/idb.h")
target_link_libraries(minstdb PUBLIC matrix CONAN_PKG::boost)

add_subdirectory(tests)
