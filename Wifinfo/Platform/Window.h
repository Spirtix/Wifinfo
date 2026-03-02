#pragma once
#include <Windows.h>
#include <functional>

class Window {
public:
	Window();
	~Window();
	[[nodiscard]] bool ShouldClose();
	[[nodiscard]] HWND GetNativeHandle() const noexcept;
	void SetResizeCallback(std::function<void(int, int)> callback);
private:
	HWND hwnd;
	WNDCLASSW wc = { };
	std::function<void(int, int)> resizeCallback;
	static LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
};