cmake_minimum_required(VERSION 3.13)
project(accumulate)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS -pthread)

add_executable(accumulate main.cpp)