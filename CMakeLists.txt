cmake_minimum_required(VERSION 3.0.0)
project(chess VERSION 0.1.0)

include_directories(inc)
file(GLOB SRC_FILES src/*.cpp)
add_executable(chess main.cpp ${SRC_FILES})

set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wpedantic -Wno-implicit-fallthrough -O0")
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)