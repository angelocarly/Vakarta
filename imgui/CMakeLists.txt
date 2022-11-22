
set(
    SOURCE_FILES
        imgui.cpp
        imgui_demo.cpp
        imgui_draw.cpp
        imgui_impl_glfw.cpp
        imgui_impl_vulkan.cpp
        imgui_tables.cpp
        imgui_widgets.cpp
)

set(
    HEADER_FILES
        imconfig.h
        imgui.h
        imgui_impl_glfw.h
        imgui_impl_vulkan.h
        imgui_internal.h
        imstb_rectpack.h
        imstb_textedit.h
        imstb_truetype.h
)

set (CMAKE_CXX_STANDARD 11)
add_library(
    imgui
    ${SOURCE_FILES}
    ${HEADER_FILES}
)

set_property(TARGET imgui PROPERTY CXX_STANDARD 17)

# Vulkan
find_package(Vulkan REQUIRED)
target_link_libraries(imgui PUBLIC Vulkan::Vulkan)

# GLFW
find_package(glfw3 3.3 REQUIRED)
target_link_libraries(imgui PUBLIC glfw)

