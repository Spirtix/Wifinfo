set(CORE_SOURCES
    main.cpp
    Application.h
    Application.cpp
    AppContext.h
)

set(WIFI_SOURCES
    Wifi/WifiAccessPoint.h
    Wifi/WifiScanner.h
    Wifi/BackgroundScanner.h
    Wifi/BackgroundScanner.cpp
)

set(WINDOW_SOURCES
    Window/View.h
    Window/TableWindow.h
    Window/TableWindow.cpp
    Window/ChannelPlotWindow.h
    Window/ChannelPlotWindow.cpp
)

set(PLATFORM_COMMON
    Platform/Renderer.h
    Platform/Window.h
)

if (WIN32)
    set(PLATFORM_SOURCES
        Platform/Win32Window.cpp
        Platform/Win32Window.h
        Platform/DirectXRenderer.cpp
        Platform/DirectXRenderer.h
        Wifi/IEParser.h
        Wifi/IEParser.cpp
        Wifi/WindowsWifiScanner.h
        Wifi/WindowsWifiScanner.cpp
    )
elseif (APPLE)
    set(PLATFORM_SOURCES
        Platform/MacWindow.mm
        Platform/MetalRenderer.mm
        Platform/MacLocationPermission.mm
        Wifi/MacWifiScanner.mm
        Platform/MetalRenderer.h
        Wifi/MacWifiScanner.h
        Platform/MacLocationPermission.h
    )
elseif (UNIX)
    set(PLATFORM_SOURCES
    )
endif()

set(ALL_SOURCES
    ${CORE_SOURCES}
    ${WIFI_SOURCES}
    ${WINDOW_SOURCES}
    ${PLATFORM_COMMON}
    ${PLATFORM_SOURCES}
)

add_executable(Wifinfo ${ALL_SOURCES})

if (APPLE)
    source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${ALL_SOURCES})
endif()

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Wifinfo PROPERTY CXX_STANDARD 20)
endif()
