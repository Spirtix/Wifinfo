#include <iostream>

#include "Application.h"
#include "Platform/Window.h"
#include "Platform/Renderer.h"
#include "Wifi/WifiScanner.h"

void Application::Run() {
	WifiScanner wifiScanner;
	std::vector<WifiAccessPoint> accessPoints = wifiScanner.ScanNetworks();
	for (const auto& ap : accessPoints) {
		std::cout << ap.ssid << " " << ap.band << " " << ap.channel << " " << ap.signalQuality << " " << ap.frequency << " " << std::endl;
	}
	Window window;
	Renderer renderer(window.GetNativeHandle());
	while (!window.ShouldClose()) {
		renderer.BeginFrame();
		renderer.EndFrame();
	}
}