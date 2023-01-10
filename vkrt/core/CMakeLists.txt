set(
    SOURCE_FILES
        Source/Camera.cpp
        Source/InputState.cpp
)

set(
    HEADER_FILES
        Camera.h
        ForwardDecl.h
        InputState.h
)

add_library(
    VkrtCore SHARED
        ${SOURCE_FILES}
        ${HEADER_FILES}
)

target_include_directories( VkrtCore PUBLIC ${PROJECT_ROOT_DIR} )

set_property( TARGET VkrtCore PROPERTY CXX_STANDARD 17 )

## Project libraries
target_link_libraries(
    VkrtCore PUBLIC
        VksAssets
        VksRender
        VksCore
        Imgui
)

### External dependencies

# Vulkan
find_package( Vulkan REQUIRED )
target_link_libraries( VkrtCore PUBLIC Vulkan::Vulkan )
set( ENV{VK_ICD_FILENAMES} "${SRC_DIR}/../lib/VulkanSDK/macOS/etc/vulkan/icd.d/MoltenVK_icd.json" )
set( ENV{VULKAN_SDK} "${SRC_DIR}/../lib/VulkanSDK/macOS/" )

# GLFW
find_package( glfw3 3.3 REQUIRED )
target_link_libraries( VkrtCore PUBLIC glfw )

# glad
target_include_directories( VkrtCore PUBLIC "${GLAD_DIR}/include" )
target_link_libraries( VkrtCore PUBLIC "glad" "${CMAKE_DL_LIBS}" )

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

# vma
if(LINUX)
    set( VMA_HPP "${LIB_DIR}/VulkanMemoryAllocator-Hpp/linux/include" )
else()
    set( VMA_HPP "${LIB_DIR}/VulkanMemoryAllocator-Hpp/mac/include" )
endif()
include_directories( ${VMA_HPP} )

# spdlog
find_package( spdlog REQUIRED )
target_link_libraries( VkrtCore PUBLIC spdlog::spdlog )

# lodepng
target_include_directories( "lodepng" PUBLIC "${LODEPNG_DIR}" )
target_link_libraries( VkrtCore PUBLIC "lodepng" )
