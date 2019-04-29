cmake_minimum_required(VERSION 3.13)
project(simple_safe_stack)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS -pthread)

add_executable(simple_safe_stack main.cpp)