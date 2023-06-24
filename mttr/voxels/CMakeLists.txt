project( Voxels VERSION 0.1.0)

set(
    Voxels_HEADER_FILES
)

set(
    Voxels_SOURCE_FILES
        Source/main.cpp
)

add_executable(
    ${PROJECT_NAME}
        ${Voxels_HEADER_FILES}
        ${Voxels_SOURCE_FILES}
)

### PATH
set( VOXELS_BIN_PATH ${CMAKE_CURRENT_BINARY_DIR} )

set_property( TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 20 )

## Project libraries
target_link_libraries(
    ${PROJECT_NAME} PUBLIC
        VkrtEngine
        VkrtGraphics
        VksRender
        VksCore
        Imgui
)

### Resources

# Copy vkrt resources
add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${RESOURCES_DIR}
        ${VOXELS_BIN_PATH}/Resources
)
add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${SHADER_DIR}
        ${VOXELS_BIN_PATH}/shaders
)

### External libraries

# Vulkan
find_package( Vulkan REQUIRED )
target_link_libraries( ${PROJECT_NAME} PUBLIC Vulkan::Vulkan )
set( ENV{VK_ICD_FILENAMES} "${SRC_DIR}/../lib/VulkanSDK/macOS/etc/vulkan/icd.d/MoltenVK_icd.json" )
set( ENV{VULKAN_SDK} "${SRC_DIR}/../lib/VulkanSDK/macOS/" )

# GLFW
find_package( glfw3 3.3 REQUIRED )
target_link_libraries( ${PROJECT_NAME} PUBLIC glfw )

# glad
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

# vma
set( VMA "${LIB_DIR}/VulkanMemoryAllocator/include" )
include_directories( ${VMA} )

# zep
target_include_directories(
        ${PROJECT_NAME}
        PUBLIC
        ${LIB_DIR}/zep/include
)
add_definitions(-DZEP_SINGLE_HEADER=1)
# Set this to get the default file system
add_definitions(-DZEP_FEATURE_CPP_FILE_SYSTEM)

# vma
if(LINUX)
    set( VMA_HPP "${LIB_DIR}/VulkanMemoryAllocator-Hpp/linux/include" )
else()
    set( VMA_HPP "${LIB_DIR}/VulkanMemoryAllocator-Hpp/mac/include" )
endif()
include_directories( ${VMA_HPP} )

# spdlog
find_package( spdlog REQUIRED )
target_link_libraries( ${PROJECT_NAME} PUBLIC spdlog::spdlog )

# lodepng
target_include_directories( "lodepng" PUBLIC "${LODEPNG_DIR}" )
target_link_libraries( ${PROJECT_NAME} PUBLIC "lodepng" )
