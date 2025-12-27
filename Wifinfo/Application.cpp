#include <iostream>

#include "Application.h"
#include "Platform/Window.h"
#include "Platform/Renderer.h"
#include "Wifi/WifiScanner.h"

void Application::Run() {
	WifiScanner wifiScanner;
	Application::AppContext.wifiAPs = wifiScanner.ScanNetworks();
	for (const auto& ap : Application::AppContext.wifiAPs) {
		std::cout << ap.ssid << " " << ap.band << " " << ap.channel << " " << ap.signalQuality << " " << ap.frequency << " " << std::endl;
	}
	
	Window window;
	Renderer renderer(window.GetNativeHandle());
	while (!window.ShouldClose()) {
		renderer.BeginFrame();
		renderer.EndFrame();
	}
}