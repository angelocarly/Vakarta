set( CMAKE_CXX_STANDARD 11 )

## IMGUI
set( IMGUI_DIR ${CMAKE_CURRENT_SOURCE_DIR}/imgui )
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

# IMNODES
set( IMNODES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/imnodes )
add_library( Imnodes STATIC )
set_target_properties( Imnodes PROPERTIES POSITION_INDEPENDENT_CODE ON )

target_sources(
    Imnodes
        PRIVATE
        ${IMNODES_DIR}/imnodes.cpp
)

target_link_libraries( Imnodes PUBLIC Imgui )

target_include_directories(
    Imnodes
        PUBLIC ${IMNODES_DIR}
)

# ImGuiFileDialog
set( IMGUIFILEDIALOG_DIR ${CMAKE_CURRENT_SOURCE_DIR}/ImGuiFileDialog )
add_library( ImGuiFileDialog STATIC )
target_link_libraries( ImGuiFileDialog PUBLIC Imgui )
target_sources(
    ImGuiFileDialog
        PRIVATE
        ${IMGUIFILEDIALOG_DIR}/ImGuiFileDialog.cpp
)
target_include_directories(
    ImGuiFileDialog
        PUBLIC ${IMGUIFILEDIALOG_DIR}
)


# Vulkan
find_package( Vulkan REQUIRED )
target_link_libraries( Imgui PUBLIC Vulkan::Vulkan )

find_package( glfw3 3.3 REQUIRED )
target_link_libraries( Imgui PUBLIC glfw )


## End imgui