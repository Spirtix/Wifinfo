#include <iostream>
#include <imgui.h>

#include "Application.h"
#include "Platform/Win32Window.h"
#include "Platform/DirectXRenderer.h"
#include "Wifi/BackgroundScanner.h"
#include "Wifi/WindowsWifiScanner.h"
#include "Window/TableWindow.h"
#include "Window/ChannelPlotWindow.h"

void Application::Run() {
	BackgroundScanner scanner(std::make_unique<WindowsWifiScanner>());
	scanner.Start();
	
	std::unique_ptr<Window> window(std::make_unique<Win32Window>());
	std::unique_ptr<Renderer> renderer(std::make_unique<DirectXRenderer>(window->GetNativeHandle()));
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