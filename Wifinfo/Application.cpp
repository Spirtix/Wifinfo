#include <iostream>

#include "Application.h"
#include "Platform/Window.h"
#include "Wifi/WifiScanner.h"

void Application::Run() {
	WifiScanner wifiScanner;
	std::vector<WifiAccessPoint> accessPoints = wifiScanner.ScanNetworks();
	for (const auto& ap : accessPoints) {
		std::cout << ap.ssid << " " << ap.band << " " << ap.channel << " " << ap.signalQuality << " " << ap.frequency << " " << std::endl;
	}
	Window window;
	while (!window.ShouldClose()) {
		Sleep(10);
	}
}