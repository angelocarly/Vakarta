set( CMAKE_CXX_STANDARD 11 )

## IMGUI

set( IMGUI_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Imgui )
add_library( Imgui STATIC )
set_target_properties( Imgui PROPERTIES POSITION_INDEPENDENT_CODE ON )

target_sources(
    Imgui
        PRIVATE
        ${IMGUI_DIR}/imgui_demo.cpp
        ${IMGUI_DIR}/imgui_draw.cpp
        ${IMGUI_DIR}/imgui_tables.cpp
        ${IMGUI_DIR}/imgui_widgets.cpp
        ${IMGUI_DIR}/imgui.cpp

        PRIVATE
        ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
        ${IMGUI_DIR}/backends/imgui_impl_vulkan.cpp
)

target_include_directories(
    Imgui
        PUBLIC ${IMGUI_DIR}
        PUBLIC ${IMGUI_DIR}/backends
)

find_package( Vulkan REQUIRED )
target_link_libraries( Imgui PUBLIC Vulkan::Vulkan )

find_package( glfw3 3.3 REQUIRED )
target_link_libraries( Imgui PUBLIC glfw )

## End imgui