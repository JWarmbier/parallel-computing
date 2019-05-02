cmake_minimum_required(VERSION 3.13)
project(cond_safe_queue)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CSS_FLAGS -pthread)

add_executable(cond_safe_queue main.cpp)