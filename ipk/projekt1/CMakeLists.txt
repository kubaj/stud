cmake_minimum_required(VERSION 3.6)
project(projekt1)

set(CMAKE_CXX_STANDARD 11)

set(SHARED_SOURCES Http.cpp Http.h FileController.cpp FileController.h IPKUtils.cpp IPKUtils.h)
set(SOURCE_FILES main.cpp ${SHARED_SOURCES})
add_executable(projekt1 ${SOURCE_FILES})
target_link_libraries(projekt1 pthread)

set(TEST_SOURCES test.cpp ${SHARED_SOURCES})
add_executable(test_main ${TEST_SOURCES})