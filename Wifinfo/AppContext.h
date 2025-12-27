#pragma once
#include <vector>
#include <memory>

#include "Wifi/WifiAccessPoint.h"
#include "Window/ImGuiWindow.h"

struct AppContext {
	std::vector<WifiAccessPoint> wifiAPs;
	std::vector<std::unique_ptr<ImGuiWindow>> windows;
	void PushWindow(std::unique_ptr<ImGuiWindow> window) {
		windows.push_back(std::move(window));
	}
};