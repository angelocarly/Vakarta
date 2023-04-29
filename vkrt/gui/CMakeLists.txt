set(
    HEADER_FILES
        BufferNode.h
        ForwardDecl.h
        GuiLayer.h
        GuiNodes.h
        GuiAttribute.h
        ImageNode.h
        ImageGenNode.h
        ImageProvider.h
        InputAttribute.h
        IdCache.h
        Link.h
        OutputAttribute.h
        ResourceProvider.h
        ShaderEditor.h
        Node.h
        ZepTheme.h
)

set(
    SOURCE_FILES
        Source/GuiLayer.cpp
        Source/GuiNodes.cpp
        Source/IdCache.cpp
        Source/ImageNode.cpp
        Source/NodeContext.cpp
        Source/Node.cpp
        Source/ImageGenNode.cpp
        Source/ShaderEditor.cpp
        Source/ZepTheme.cpp
)

add_library(
    VkrtGui
        ${SOURCE_FILES}
        ${HEADER_FILES}
)

## Project libraries
target_link_libraries(
    VkrtGui PUBLIC
        VkrtGraphics
        VksRender
        VksCore
        Imgui
)

set_property( TARGET VkrtGui PROPERTY CXX_STANDARD 17 )

### External dependencies

# Vulkan
find_package( Vulkan REQUIRED )
target_link_libraries( VkrtGui PUBLIC Vulkan::Vulkan )
set( ENV{VK_ICD_FILENAMES} "${SRC_DIR}/../lib/VulkanSDK/macOS/etc/vulkan/icd.d/MoltenVK_icd.json" )
set( ENV{VULKAN_SDK} "${SRC_DIR}/../lib/VulkanSDK/macOS/" )

# GLFW
find_package( glfw3 3.3 REQUIRED )
target_link_libraries( VkrtGui PUBLIC glfw )

# glad
target_include_directories( VkrtGui PUBLIC "${GLAD_DIR}/include" )
target_link_libraries( VkrtGui PUBLIC "glad" "${CMAKE_DL_LIBS}" )

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
    VkrtGui
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
target_link_libraries( VkrtGui PUBLIC spdlog::spdlog )

# lodepng
target_include_directories( "lodepng" PUBLIC "${LODEPNG_DIR}" )
target_link_libraries( VkrtGui PUBLIC "lodepng" )
