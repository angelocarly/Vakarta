set(
    SOURCE_FILES
        Source/Buffer.cpp
        Source/Image.cpp
        Source/Instance.cpp
        Source/Device.cpp
        Source/Mesh.cpp
        Source/PhysicalDevice.cpp
        Source/Pipeline.cpp
        Source/RenderPass.cpp
        Source/Swapchain.cpp
        Source/Utils.cpp
        Source/Vertex.cpp
)

set(
    HEADER_FILES
        Buffer.h
        Image.h
        ForwardDecl.h
        Instance.h
        Device.h
        Mesh.h
        PhysicalDevice.h
        Pipeline.h
        RenderPass.h
        Swapchain.h
        Utils.h
        Vertex.h
)

set( CMAKE_CXX_STANDARD 11)
add_library(
    vks SHARED
    ${SOURCE_FILES}
    ${HEADER_FILES}
)
target_include_directories( vks PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/.. )

set_property( TARGET vks PROPERTY CXX_STANDARD 17 )

# Vulkan
find_package(Vulkan REQUIRED)
target_link_libraries(vks PUBLIC Vulkan::Vulkan)
set(ENV{VK_ICD_FILENAMES} "${SRC_DIR}/../lib/VulkanSDK/macOS/etc/vulkan/icd.d/MoltenVK_icd.json")
set(ENV{VULKAN_SDK} "${SRC_DIR}/../lib/VulkanSDK/macOS/")

# VMA
target_include_directories( vks PUBLIC ${LIB_DIR}/VulkanMemoryAllocator/include )

# GLFW
find_package(glfw3 3.3 REQUIRED)
target_link_libraries(vks PUBLIC glfw)

# spdlog
find_package(spdlog REQUIRED)
target_link_libraries(vks PUBLIC spdlog::spdlog)
