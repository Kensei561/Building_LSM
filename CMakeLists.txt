cmake_minimum_required(VERSION 3.14)
project(LSMStorageEngine CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(src)

# Library target for non-template implementation files
add_library(lsm_core src/arena.cpp)

# Test executable
add_executable(skiplist_test tests/skiplist_test.cpp)
target_link_libraries(skiplist_test PRIVATE lsm_core)