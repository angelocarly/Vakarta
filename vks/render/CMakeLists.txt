set(
    HEADER_FILES
        ComputePipeline.h
        DescriptorBinder.h
        DescriptorLayoutBuilder.h
        DescriptorSet.h
        Device.h
        ForwardDecl.h
        GuiPass.h
        Instance.h
        Mesh.h
        OffScreenRenderPass.h
        PhysicalDevice.h
        Pipeline.h
        RenderPass.h
        Swapchain.h
        Utils.h
        VksSession.h
        Window.h
)

set(
    SOURCE_FILES
        Source/ComputePipeline.cpp
        Source/DescriptorBinder.cpp
        Source/DescriptorLayoutBuilder.cpp
        Source/DescriptorSet.cpp
        Source/Device.cpp
        Source/GuiPass.cpp
        Source/Instance.cpp
        Source/Mesh.cpp
        Source/OffScreenRenderPass.cpp
        Source/PhysicalDevice.cpp
        Source/Pipeline.cpp
        Source/RenderPass.cpp
        Source/Swapchain.cpp
        Source/Utils.cpp
        Source/VulkanSession.cpp
        Source/Window.cpp
)

add_library(
    VksRender SHARED
    ${SOURCE_FILES}
    ${HEADER_FILES}
)

set_property( TARGET VksRender PROPERTY CXX_STANDARD 17 )

target_include_directories( VksRender PUBLIC ${PROJECT_ROOT_DIR} )
target_include_directories( VksRender PUBLIC ${LIB_DIR} )

# Project libraries
target_link_libraries(
    VksRender PUBLIC
        Imgui
        VksAssets
        VksCore
)

# Vulkan
find_package(Vulkan REQUIRED)
target_link_libraries( VksRender PUBLIC Vulkan::Vulkan)
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

# Imnodes
target_link_libraries( VksRender PUBLIC Imnodes )

# GLFW
find_package(glfw3 3.3 REQUIRED)
target_link_libraries( VksRender PUBLIC glfw)

# spdlog
find_package(spdlog REQUIRED)
target_link_libraries( VksRender PUBLIC spdlog::spdlog)
