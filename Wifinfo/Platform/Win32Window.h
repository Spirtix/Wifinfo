#pragma once
#include <Windows.h>
#include <functional>

#include "Window.h"

class Win32Window : public Window {
public:
	Win32Window();
	~Win32Window() override;
	[[nodiscard]] bool ShouldClose() override;
	[[nodiscard]] NativeHandle GetNativeHandle() const noexcept override;
	void SetResizeCallback(std::function<void(int, int)> callback) override;
private:
	HWND hwnd;
	WNDCLASSW wc = { };
	std::function<void(int, int)> resizeCallback;
	static LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
};