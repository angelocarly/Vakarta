project( Voxels VERSION 0.1.0)

set(
    Voxels_HEADER_FILES
        VoxelCompute.h
        VoxelControls.h
        VoxelEngine.h
        VoxelPresenter.h
        VoxelRenderer.h
)

set(
    Voxels_SOURCE_FILES
        Source/VoxelCompute.cpp
        Source/VoxelControls.cpp
        Source/VoxelEngine.cpp
        Source/VoxelPresenter.cpp
        Source/VoxelRenderer.cpp
)

set(
    Voxels_GLSL_FILES
        shaders/Voxels.frag
        shaders/Voxels.comp
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

# Shaders
if(LINUX)
    set( GLSL_VALIDATOR "/usr/bin/glslangValidator" )
else()
    set( GLSL_VALIDATOR "/usr/local/bin/glslangValidator" )
endif()

set( SHADER_DIR "${VOXELS_BIN_PATH}/shaders" )

foreach( GLSL ${Voxels_GLSL_FILES} )
    get_filename_component( FILE_NAME ${GLSL} NAME )
    set(
        SPIRV
            "${SHADER_DIR}/${FILE_NAME}.spv"
    )
    message( ${SPIRV} )
    message( ${CMAKE_SOURCE_DIR}/mttr/voxels/${GLSL} )
    add_custom_command(
            OUTPUT ${SPIRV}
            COMMAND ${CMAKE_COMMAND} -E make_directory "${SHADER_DIR}"
            COMMAND ${GLSL_VALIDATOR} --target-env vulkan1.0 -V ${CMAKE_SOURCE_DIR}/mttr/voxels/${GLSL} -o ${SPIRV}
            DEPENDS ${GLSL} )
    list( APPEND SPIRV_BINARY_FILES ${SPIRV} )
endforeach( GLSL )

add_custom_target(
    VoxelsShaders
        DEPENDS ${SPIRV_BINARY_FILES}
)

add_dependencies( ${PROJECT_NAME} VoxelsShaders )

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
