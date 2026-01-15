add_executable (Wifinfo "main.cpp" "Wifi/WifiAccessPoint.h" "Wifi/WifiScanner.h" "Wifi/WifiScanner.cpp" "Application.h" "Application.cpp" "Window/ImGuiWindow.h" "Platform/Window.cpp" "Platform/Renderer.cpp" "Platform/Renderer.h" "Window/ImGuiWindow.h" "Window/TableWindow.h" "Window/TableWindow.cpp" "AppContext.h" "Window/ChannelPlotWindow.h" "Window/ChannelPlotWindow.cpp")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Wifinfo PROPERTY CXX_STANDARD 20)
endif()
