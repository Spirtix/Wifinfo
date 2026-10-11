cmake_minimum_required (VERSION 3.20)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("Wifinfo")
include_directories(${CMAKE_SOURCE_DIR}/vendor/imgui)
include_directories(${CMAKE_SOURCE_DIR}/vendor/implot)
add_subdirectory ("Wifinfo")
add_subdirectory("vendor")

if (WIN32)
target_link_libraries(Wifinfo PRIVATE wlanapi d3d11 imgui)
elseif (APPLE)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_OBJCXX_STANDARD 20)

find_library(COCOA_FRAMEWORK Cocoa)
find_library(METAL_FRAMEWORK Metal)
find_library(METALKIT_FRAMEWORK MetalKit)
find_library(QUARTZCORE_FRAMEWORK QuartzCore)
find_library(GAMECONTROLLER_FRAMEWORK GameController)
find_library(COREWLAN_FRAMEWORK CoreWLAN)
find_library(CORELOCATION_FRAMEWORK CoreLocation)

target_link_libraries(Wifinfo PRIVATE ${COCOA_FRAMEWORK} ${METAL_FRAMEWORK} ${METALKIT_FRAMEWORK} ${QUARTZCORE_FRAMEWORK} ${GAMECONTROLLER_FRAMEWORK} ${COREWLAN_FRAMEWORK} ${CORELOCATION_FRAMEWORK} imgui)
set_target_properties(Wifinfo PROPERTIES MACOSX_BUNDLE TRUE MACOSX_BUNDLE_INFO_PLIST ${CMAKE_SOURCE_DIR}/Wifinfo/Info.plist)
endif()
