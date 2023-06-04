set(
    ShaderNodes_Gui_HEADER_FILES
        gui/BufferNode.h
        gui/ColorFuncShaderNode.h
        gui/CrystalShaderNode.h
        gui/FilterNode.h
        gui/ForwardDecl.h
        gui/GuiLayer.h
        gui/GuiNodes.h
        gui/GuiAttribute.h
        gui/Graph.h
        gui/ImageNode.h
        gui/ImageProvider.h
        gui/InputAttribute.h
        gui/IdCache.h
        gui/Link.h
        gui/Node.h
        gui/OutputAttribute.h
        gui/PNGNode.h
        gui/ResourceProvider.h
        gui/ShaderEditor.h
        gui/ShaderNode.h
        gui/UpdateListener.h
        gui/ZepTheme.h
)

set(
    ShaderNodes_Gui_SOURCE_FILES
        gui/Source/ColorFuncShaderNode.cpp
        gui/Source/CrystalShaderNode.cpp
        gui/Source/FilterNode.cpp
        gui/Source/GuiLayer.cpp
        gui/Source/GuiNodes.cpp
        gui/Source/IdCache.cpp
        gui/Source/ImageNode.cpp
        gui/Source/NodeContext.cpp
        gui/Source/Node.cpp
        gui/Source/PNGNode.cpp
        gui/Source/ShaderEditor.cpp
        gui/Source/ShaderNode.cpp
        gui/Source/ZepTheme.cpp
)

set(
    ShaderNodes_Graphics_HEADER_FILES
        graphics/CrystalViewShader.h
        graphics/FilterShader.h
        graphics/GeoGenPipeline.h
        graphics/LinePipeline.h
        graphics/MeshPipeline.h
        graphics/Presenter.h
        graphics/ReflectionPresenter.h
)

set(
    ShaderNodes_Graphics_SOURCE_FILES
        graphics/Source/CrystalViewShader.cpp
        graphics/Source/FilterShader.cpp
        graphics/Source/GeoGenPipeline.cpp
        graphics/Source/LinePipeline.cpp
        graphics/Source/MeshPipeline.cpp
        graphics/Source/ReflectionPresenter.cpp
)

add_library(
    VpsShaderNodes
        ${ShaderNodes_Gui_HEADER_FILES}
        ${ShaderNodes_Gui_SOURCE_FILES}
        ${ShaderNodes_Graphics_HEADER_FILES}
        ${ShaderNodes_Graphics_SOURCE_FILES}
        ${HEADER_FILES}
)

## Project libraries
target_link_libraries(
    VpsShaderNodes PUBLIC
        VkrtGraphics
        VksRender
        VksCore
        Imgui
)

set_property( TARGET VpsShaderNodes PROPERTY CXX_STANDARD 20 )

### External dependencies
target_link_libraries( VpsShaderNodes PUBLIC ImGuiFileDialog )

# Vulkan
find_package( Vulkan REQUIRED )
target_link_libraries( VpsShaderNodes PUBLIC Vulkan::Vulkan )
set( ENV{VK_ICD_FILENAMES} "${SRC_DIR}/../lib/VulkanSDK/macOS/etc/vulkan/icd.d/MoltenVK_icd.json" )
set( ENV{VULKAN_SDK} "${SRC_DIR}/../lib/VulkanSDK/macOS/" )

# GLFW
find_package( glfw3 3.3 REQUIRED )
target_link_libraries( VpsShaderNodes PUBLIC glfw )

# glad
target_include_directories( VpsShaderNodes PUBLIC "${GLAD_DIR}/include" )
target_link_libraries( VpsShaderNodes PUBLIC "glad" "${CMAKE_DL_LIBS}" )

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
target_link_libraries( VpsShaderNodes PUBLIC spdlog::spdlog )

# lodepng
target_include_directories( "lodepng" PUBLIC "${LODEPNG_DIR}" )
target_link_libraries( VpsShaderNodes PUBLIC "lodepng" )
