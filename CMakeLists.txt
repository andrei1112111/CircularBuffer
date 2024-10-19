cmake_minimum_required(VERSION 3.26)
project(CircularBuffer)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

enable_testing()

include_directories(./)

add_library(CircularBuffer_Lib SHARED CircularBuffer.h)

set_target_properties(CircularBuffer_Lib PROPERTIES LINKER_LANGUAGE CXX)

add_subdirectory(test)

add_executable(CircularBuffer main.cpp)

target_link_libraries(CircularBuffer PRIVATE CircularBuffer_Lib)
