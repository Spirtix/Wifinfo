#include <iostream>

#include "Application.h"
#include "Platform/Window.h"
#include "Platform/Renderer.h"
#include "Wifi/BackgroundScanner.h"
#include "Wifi/WifiScanner.h"
#include "Window/TableWindow.h"
#include "Window/ChannelPlotWindow.h"

void Application::Run() {
	BackgroundScanner scanner(std::make_unique<WifiScanner>());
	scanner.Start();
	
	Window window;
	Renderer renderer(window.GetNativeHandle());
	window.SetResizeCallback([&renderer](int width, int height) {
		renderer.OnResize(width, height);
	});
	Application::AppContext.PushWindow(std::make_unique<TableWindow>());
	Application::AppContext.PushWindow(std::make_unique<ChannelPlotWindow>());
	while (!window.ShouldClose()) {
		if (auto scanResult = scanner.TakeResults())
			Application::AppContext.wifiAPs = std::move(*scanResult);
		renderer.BeginFrame();
		for (auto& wnd : Application::AppContext.windows) {
			wnd->Update();
		}
		renderer.EndFrame();
	}

	scanner.Stop();
}