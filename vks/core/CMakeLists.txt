set(
    HEADER_FILES
        Buffer.h
        Vertex.h
)

set(
    SOURCE_FILES
        Source/Buffer.cpp
        Source/Vertex.cpp
)

set( CMAKE_CXX_STANDARD 11)
add_library(
    VksCore SHARED
    ${SOURCE_FILES}
    ${HEADER_FILES}
)
set_property( TARGET VksCore PROPERTY CXX_STANDARD 17 )

target_include_directories( VksCore PUBLIC ${PROJECT_ROOT_DIR} )
target_include_directories( VksCore PUBLIC ${LIB_DIR} )

# Vulkan
find_package(Vulkan REQUIRED)
target_link_libraries( VksCore PUBLIC Vulkan::Vulkan)
set(ENV{VK_ICD_FILENAMES} "${SRC_DIR}/../lib/VulkanSDK/macOS/etc/vulkan/icd.d/MoltenVK_icd.json")
set(ENV{VULKAN_SDK} "${SRC_DIR}/../lib/VulkanSDK/macOS/")

# VMA
include_directories( ${LIB_DIR}/VulkanMemoryAllocator/include )

# VMA-hpp
if(LINUX)
    set( VMA_HPP "${LIB_DIR}/VulkanMemoryAllocator-Hpp/linux/include" )
else()
    set( VMA_HPP "${LIB_DIR}/VulkanMemoryAllocator-Hpp/mac/include" )
endif()
include_directories( ${VMA_HPP} )

# spdlog
find_package(spdlog REQUIRED)
target_link_libraries( VksCore PUBLIC spdlog::spdlog)
