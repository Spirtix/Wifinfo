#pragma once
#include <vector>

#include "Wifi/WifiAccessPoint.h"
#include "Window/ImGuiWindow.h"

struct AppContext {
	std::vector<WifiAccessPoint> wifiAPs;
	std::vector<ImGuiWindow> windows;
};