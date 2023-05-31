### Project

# Project definition
cmake_minimum_required(VERSION 3.3)
project(VKRT VERSION 0.1.0)

set(PROJECT_ROOT_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
set(LIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/lib")

### PATH
set( VKRT_BIN_PATH ${CMAKE_CURRENT_BINARY_DIR}/vkrt/application )

### Libraries
# glad
set( GLAD_DIR "${LIB_DIR}/glad" )
add_library( "glad" "${GLAD_DIR}/src/glad.c" )
target_include_directories( "glad" PUBLIC "${GLAD_DIR}/include" )

#lodepng
set( LODEPNG_DIR "${LIB_DIR}/lodepng" )
add_library( "lodepng" ${LODEPNG_DIR}/lodepng.cpp )

### Subprojects
add_subdirectory(lib)
add_subdirectory(vks)
add_subdirectory(vkrt)
add_subdirectory(vps)