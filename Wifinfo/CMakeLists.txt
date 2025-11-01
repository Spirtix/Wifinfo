add_executable (Wifinfo "main.cpp" "Wifi/WifiAccessPoint.h" "Wifi/WifiScanner.h" "Wifi/WifiScanner.cpp" "Application.h" "Application.cpp" "Platform/Window.h" "Platform/Window.cpp")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Wifinfo PROPERTY CXX_STANDARD 20)
endif()
