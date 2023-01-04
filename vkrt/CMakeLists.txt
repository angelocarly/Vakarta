set(
    SOURCE_FILES
        Source/Camera.cpp
        Source/Engine.cpp
        Source/InputState.cpp
        Source/main.cpp
        Source/Renderer.cpp
)

set(
    HEADER_FILES
        Camera.h
        Engine.h
        ForwardDecl.h
        InputState.h
        Renderer.h
)

set( CMAKE_CXX_STANDARD 11 )
add_executable(
    ${PROJECT_NAME}
        ${SOURCE_FILES}
        ${HEADER_FILES}
)
target_include_directories( ${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/.. )
set_property( TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17 )

target_link_libraries(
    ${PROJECT_NAME} PUBLIC
        vks
)

### Shaders

# if(NOT LINUX) should work, too, if you need that
if(LINUX)
	set( GLSL_VALIDATOR "/usr/bin/glslangValidator" )
    set( EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR} )
else()
	set( GLSL_VALIDATOR "/usr/local/bin/glslangValidator" )
    set( EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR}/Debug )
endif()

file(
    GLOB_RECURSE GLSL_SOURCE_FILES
        "shaders/*.frag"
        "shaders/*.vert"
        "shaders/*.comp"
)

foreach( GLSL ${GLSL_SOURCE_FILES} )
    get_filename_component( FILE_NAME ${GLSL} NAME )
    set( SPIRV "${EXECUTABLE_OUTPUT_PATH}/shaders/${FILE_NAME}.spv" )
    message( ${SPIRV} )
    add_custom_command(
            OUTPUT ${SPIRV}
            COMMAND ${CMAKE_COMMAND} -E make_directory "${EXECUTABLE_OUTPUT_PATH}/shaders"
            COMMAND ${GLSL_VALIDATOR} --target-env vulkan1.0 -V ${GLSL} -o ${SPIRV}
            DEPENDS ${GLSL} )
    list( APPEND SPIRV_BINARY_FILES ${SPIRV} )
endforeach( GLSL )

add_custom_target(
    Shaders
        DEPENDS ${SPIRV_BINARY_FILES}
)

add_dependencies( ${PROJECT_NAME} Shaders )

### Resources
add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory "${EXECUTABLE_OUTPUT_PATH}/resources"
)
add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_SOURCE_DIR}/resources/teapot.obj
        ${EXECUTABLE_OUTPUT_PATH}/resources/teapot.obj
)

### External dependencies

# Vulkan
find_package( Vulkan REQUIRED )
target_link_libraries( VKRT PUBLIC Vulkan::Vulkan )
set( ENV{VK_ICD_FILENAMES} "${SRC_DIR}/../lib/VulkanSDK/macOS/etc/vulkan/icd.d/MoltenVK_icd.json" )
set( ENV{VULKAN_SDK} "${SRC_DIR}/../lib/VulkanSDK/macOS/" )

# GLFW
find_package( glfw3 3.3 REQUIRED )
target_link_libraries( ${PROJECT_NAME} PUBLIC glfw )

# glad
set( GLAD_DIR "${LIB_DIR}/glad" )
add_library( "glad" "${GLAD_DIR}/src/glad.c" )
target_include_directories( "glad" PUBLIC "${GLAD_DIR}/include" )
target_include_directories( ${PROJECT_NAME} PUBLIC "${GLAD_DIR}/include" )
target_link_libraries( ${PROJECT_NAME} PUBLIC "glad" "${CMAKE_DL_LIBS}" )

# glm
find_package( glm REQUIRED )
include_directories( ${GLM_INCLUDE_DIRS} )
link_directories( ${GLM_LIBRARY_DIRS} )
add_definitions( ${GLM_DEFINITIONS} )

# stb
set( STB_DIR "${LIB_DIR}/stb" )
include_directories( ${STB_DIR} )

# tinyobjloader
set( TINYOBJLOADER_DIR "${LIB_DIR}/tinyobjloader" )
include_directories( ${TINYOBJLOADER_DIR} )

# vma
set( VMA "${LIB_DIR}/VulkanMemoryAllocator/include" )
include_directories( ${VMA} )

# vma
set( VMA_HPP "${LIB_DIR}/VulkanMemoryAllocator-Hpp/include" )
include_directories( ${VMA_HPP} )

# spdlog
find_package( spdlog REQUIRED )
target_link_libraries( ${PROJECT_NAME} PUBLIC spdlog::spdlog )

# lodepng
set( LODEPNG_DIR "${LIB_DIR}/lodepng" )
add_library( "lodepng" ${LODEPNG_DIR}/lodepng.cpp )
target_include_directories( "lodepng" PUBLIC "${LODEPNG_DIR}" )
target_link_libraries( ${PROJECT_NAME} PUBLIC "lodepng" )
