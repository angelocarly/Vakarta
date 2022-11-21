### Project

# Project definition
cmake_minimum_required(VERSION 3.1)
project(VKRT VERSION 0.1.0)

# Source files
set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(LIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/lib")
file(GLOB_RECURSE SOURCE_FILES "src/*" "src/*.cpp")
set(SOURCES ${SOURCE_FILES}
        src/vks/Source/VksInstance.cpp
        src/vks/Source/VksPhysicalDevice.cpp
        src/vks/Source/VksLogicalDevice.cpp
    src/engine.cpp
)
set(HEADERS
        src/vks/VksInstance.h
        src/vks/VksPhysicalDevice.h
        src/vks/VksDevice.h
    src/engine.h
)

# Executable definition and properties
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})
target_include_directories(${PROJECT_NAME} PUBLIC "${SRC_DIR}")
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)

### Shaders

# Copy and compile shaders
if (${CMAKE_HOST_SYSTEM_PROCESSOR} STREQUAL "AMD64")
    set(GLSL_VALIDATOR "$ENV{VULKAN_SDK}/usr/bin/glslangValidator")
else()
    set(GLSL_VALIDATOR "$ENV{VULKAN_SDK}/usr/bin/glslangValidator")
endif()

file(GLOB_RECURSE GLSL_SOURCE_FILES
        "shaders/*.frag"
        "shaders/*.vert"
        "shaders/*.comp"
        )

foreach(GLSL ${GLSL_SOURCE_FILES})
    get_filename_component(FILE_NAME ${GLSL} NAME)
    set(SPIRV "${PROJECT_BINARY_DIR}/shaders/${FILE_NAME}.spv")
    add_custom_command(
            OUTPUT ${SPIRV}
            COMMAND ${CMAKE_COMMAND} -E make_directory "${PROJECT_BINARY_DIR}/shaders/"
            COMMAND ${GLSL_VALIDATOR} --target-env vulkan1.0 -V ${GLSL} -o ${SPIRV}
            DEPENDS ${GLSL})
    list(APPEND SPIRV_BINARY_FILES ${SPIRV})
endforeach(GLSL)

add_custom_target(
        Shaders
        DEPENDS ${SPIRV_BINARY_FILES}
)

add_dependencies(${PROJECT_NAME} Shaders)

add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:VKRT>/shaders/"
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${PROJECT_BINARY_DIR}/shaders"
        "$<TARGET_FILE_DIR:VKRT>/shaders"
)

### External dependencies

# Vulkan
find_package(Vulkan REQUIRED)
target_link_libraries(VKRT PUBLIC Vulkan::Vulkan)
#target_link_libraries(tests PUBLIC Vulkan::Vulkan)
set(ENV{VK_ICD_FILENAMES} "${SRC_DIR}/../lib/VulkanSDK/macOS/etc/vulkan/icd.d/MoltenVK_icd.json")
set(ENV{VULKAN_SDK} "${SRC_DIR}/../lib/VulkanSDK/macOS/")

# GLFW
find_package(glfw3 3.3 REQUIRED)
target_link_libraries(${PROJECT_NAME} PUBLIC glfw)

# glad
set(GLAD_DIR "${LIB_DIR}/glad")
add_library("glad" "${GLAD_DIR}/src/glad.c")
target_include_directories("glad" PUBLIC "${GLAD_DIR}/include")
target_include_directories(${PROJECT_NAME} PUBLIC "${GLAD_DIR}/include")
target_link_libraries(${PROJECT_NAME} PUBLIC "glad" "${CMAKE_DL_LIBS}")

# glm
find_package(glm REQUIRED)
include_directories(${GLM_INCLUDE_DIRS})
link_directories(${GLM_LIBRARY_DIRS})
add_definitions(${GLM_DEFINITIONS})

# stb
set(STB_DIR "${LIB_DIR}/stb")
include_directories(${STB_DIR})

# spdlog
find_package(spdlog REQUIRED)
target_link_libraries(${PROJECT_NAME} PUBLIC spdlog::spdlog)

# lodepng
set(LODEPNG_DIR "${LIB_DIR}/lodepng")
add_library("lodepng" ${LODEPNG_DIR}/lodepng.cpp)
target_include_directories("lodepng" PUBLIC "${LODEPNG_DIR}")
target_link_libraries(${PROJECT_NAME} PUBLIC "lodepng")
