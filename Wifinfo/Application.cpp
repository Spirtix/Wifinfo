#include <iostream>
#include <imgui.h>

#include "Application.h"
#include "Platform/Window.h"
#if defined(_WIN32)
#include "Platform/Win32Window.h"
#include "Platform/DirectXRenderer.h"
#include "Wifi/WindowsWifiScanner.h"
#endif
#if defined(__APPLE__)
#include "Platform/MacWindow.h"
#include "Platform/MetalRenderer.h"
#include "Platform/MacLocationPermission.h"
#include "Wifi/MacWifiScanner.h"
#endif
#include "Wifi/BackgroundScanner.h"
#include "Window/TableWindow.h"
#include "Window/ChannelPlotWindow.h"

void Application::Run() {
#if defined(_WIN32)
	std::unique_ptr<Window> window(std::make_unique<Win32Window>());
	std::unique_ptr<Renderer> renderer(std::make_unique<DirectXRenderer>(window->GetNativeHandle()));
    BackgroundScanner scanner(std::make_unique<WindowsWifiScanner>());
#elif defined(__APPLE__)
    MacLocationPermission::Request();
	std::unique_ptr<Window> window(std::make_unique<MacWindow>());
	std::unique_ptr<Renderer> renderer(std::make_unique<MetalRenderer>(window->GetNativeHandle()));
    BackgroundScanner scanner(std::make_unique<MacWifiScanner>());
#endif
    scanner.Start();
	window->SetResizeCallback([&renderer](int width, int height) {
		renderer->OnResize(width, height);
	});
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

	appContext.PushWindow(std::make_unique<TableWindow>(appContext));
	appContext.PushWindow(std::make_unique<ChannelPlotWindow>(appContext));
	while (!window->ShouldClose()) {
		if (auto scanResult = scanner.TakeResults())
		 	appContext.wifiAPs = std::move(*scanResult);
		renderer->BeginFrame();
		ImGuiViewport* vp = ImGui::GetMainViewport();
		ImGui::DockSpaceOverViewport(vp->ID, vp);
		for (auto& wnd : appContext.windows) {
			wnd->Update();
		}
		renderer->EndFrame();
	}

	scanner.Stop();
}
