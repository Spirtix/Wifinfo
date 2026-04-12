#include <iostream>
#include <imgui.h>

#include "Application.h"
#include "Platform/Window.h"
#if defined(_WIN32)
#include "Platform/DirectXRenderer.h"
#endif
#if defined(__APPLE__)
#include "Platform/MetalRenderer.h"
#endif
#include "Wifi/BackgroundScanner.h"
#if defined(_WIN32)
#include "Wifi/WindowsWifiScanner.h"
#endif
#include "Window/TableWindow.h"
#include "Window/ChannelPlotWindow.h"

void Application::Run() {
	// BackgroundScanner scanner(std::make_unique<WindowsWifiScanner>());
	// scanner.Start();
	
	Window window;
#if defined(_WIN32)
	std::unique_ptr<Renderer> renderer(std::make_unique<DirectXRenderer>(window.GetNativeHandle()));
#elif defined(__APPLE__)
	std::unique_ptr<Renderer> renderer(std::make_unique<MetalRenderer>(window.GetNativeHandle()));
#endif
	window.SetResizeCallback([&renderer](int width, int height) {
		renderer->OnResize(width, height);
	});
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

	appContext.PushWindow(std::make_unique<TableWindow>(appContext));
	appContext.PushWindow(std::make_unique<ChannelPlotWindow>(appContext));
	while (!window.ShouldClose()) {
		// if (auto scanResult = scanner.TakeResults())
		// 	appContext.wifiAPs = std::move(*scanResult);
		renderer->BeginFrame();
		ImGuiViewport* vp = ImGui::GetMainViewport();
		ImGui::DockSpaceOverViewport(vp->ID, vp);
		for (auto& wnd : appContext.windows) {
			wnd->Update();
		}
		renderer->EndFrame();
	}

	// scanner.Stop();
}