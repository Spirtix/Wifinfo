add_executable (Wifinfo "main.cpp" "Wifi/WifiAccessPoint.h" "Wifi/WindowsWifiScanner.h" "Wifi/WindowsWifiScanner.cpp" "Application.h" "Application.cpp" "Window/View.h" "Platform/Window.cpp" "Platform/DirectXRenderer.cpp" "Platform/DirectXRenderer.h" "Window/TableWindow.h" "Window/TableWindow.cpp" "AppContext.h" "Window/ChannelPlotWindow.h" "Window/ChannelPlotWindow.cpp" "Wifi/BackgroundScanner.h" "Wifi/BackgroundScanner.cpp" "Wifi/IEParser.h" "Wifi/IEParser.cpp" "Wifi/WifiScanner.h")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Wifinfo PROPERTY CXX_STANDARD 20)
endif()
