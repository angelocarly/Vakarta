set(
    HEADER_FILES
        Engine.h
        GuiPanels.h
)

set(
    SOURCE_FILES
        Source/Engine.cpp
        Source/GuiPanels.cpp
        Source/main.cpp
)

add_executable(
    ${PROJECT_NAME}
        ${SOURCE_FILES}
        ${HEADER_FILES}
)

target_include_directories( ${PROJECT_NAME} PUBLIC ${PROJECT_ROOT_DIR} )

set_property( TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17 )

## Project libraries
target_link_libraries(
    ${PROJECT_NAME} PUBLIC
        VkrtGraphics
        VksAssets
        VksRender
        VksCore
        Imgui
)

### Resources
set(
    RESOURCES
        teapot.obj
        bunny.obj
        mine.obj
        image.png
)
add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory "${VKRT_BIN_PATH}/resources"
)
foreach( RESOURCE ${RESOURCES} )
    add_custom_command(
            TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy
            ${CMAKE_SOURCE_DIR}/resources/${RESOURCE}
            ${VKRT_BIN_PATH}/resources/${RESOURCE}
    )
endforeach( RESOURCE )

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
