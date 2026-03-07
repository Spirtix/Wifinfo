#pragma once
#include <vector>
#include <memory>

#include "Wifi/WifiAccessPoint.h"
#include "Window/View.h"

struct AppContext {
	std::vector<WifiAccessPoint> wifiAPs;
	std::vector<std::unique_ptr<View>> windows;
	void PushWindow(std::unique_ptr<View> window) {
		windows.push_back(std::move(window));
	}
};