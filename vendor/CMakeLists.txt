set(IMGUI_SOURCES
    imgui/imgui.cpp
    imgui/imgui_draw.cpp
    imgui/imgui_tables.cpp
    imgui/imgui_widgets.cpp
    imgui/imgui_demo.cpp
    implot/implot.cpp
    implot/implot_items.cpp
)

# Platform-specific backends
if (WIN32)
    list(APPEND IMGUI_SOURCES
        imgui/backends/imgui_impl_win32.cpp
        imgui/backends/imgui_impl_dx11.cpp
    )
elseif (APPLE)
    list(APPEND IMGUI_SOURCES
        imgui/backends/imgui_impl_metal.mm
        imgui/backends/imgui_impl_osx.mm
    )
elseif (UNIX)
    list(APPEND IMGUI_SOURCES
        imgui/backends/imgui_impl_glfw.cpp
        imgui/backends/imgui_impl_opengl3.cpp
    )
endif()

add_library(imgui STATIC ${IMGUI_SOURCES})