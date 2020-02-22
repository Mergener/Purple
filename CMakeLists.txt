cmake_minimum_required (VERSION 3.10)

set (${CMAKE_BINARY_DIR} builds)

project (Purple)

# Use C++17
set (CMAKE_CXX_STANDARD 17)

include_directories(global/include)

add_subdirectory(global)
add_subdirectory(vm)
add_subdirectory(purlibs)
add_subdirectory(standalone)
add_subdirectory(compilers)