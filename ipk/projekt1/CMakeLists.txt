cmake_minimum_required(VERSION 3.6)
project(projekt1)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp http.cpp http.h)
add_executable(projekt1 ${SOURCE_FILES})
target_link_libraries(projekt1 pthread)