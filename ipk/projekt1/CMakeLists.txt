cmake_minimum_required(VERSION 3.6)
project(projekt1)

set(CMAKE_CXX_STANDARD 11)

set(SHARED_SOURCES Http.cpp Http.h FileController.cpp FileController.h IPKUtils.cpp IPKUtils.h)
set(SERVER_SOURCE_FILES server.cpp ${SHARED_SOURCES})
set(CLIENT_SOURCE_FILES client.cpp ${SHARED_SOURCES})

add_executable(ftrestd ${SERVER_SOURCE_FILES})
target_link_libraries(ftrestd pthread)

add_executable(ftrest ${CLIENT_SOURCE_FILES})