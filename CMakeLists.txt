### Project

# Project definition
cmake_minimum_required(VERSION 3.3)
project(VKRT VERSION 0.1.0)

set(LIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/lib")

add_subdirectory(lib)
add_subdirectory(vks)
add_subdirectory(vkrt)