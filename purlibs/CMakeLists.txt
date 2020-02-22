add_library(Purlib_io STATIC io.cpp)
target_include_directories(Purlib_io PUBLIC ${CMAKE_SOURCE_DIR}/vm/export/include)