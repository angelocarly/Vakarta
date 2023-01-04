set( CMAKE_CXX_STANDARD 11 )

## IMGUI

set( IMGUI_DIR ${CMAKE_CURRENT_SOURCE_DIR}/imgui )
add_library( imgui STATIC )
set_target_properties( imgui PROPERTIES POSITION_INDEPENDENT_CODE ON )

target_sources(
    imgui
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
    imgui
        PUBLIC ${IMGUI_DIR}
        PUBLIC ${IMGUI_DIR}/backends
)