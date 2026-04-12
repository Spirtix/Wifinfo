#pragma once
#if defined(_WIN32)
#include <Windows.h>
#endif
#include <functional>

class Window {
public:
	Window();
	~Window();
	[[nodiscard]] bool ShouldClose();
#if defined(_WIN32)
	[[nodiscard]] HWND GetNativeHandle() const noexcept;
#elif defined(__APPLE__)
	[[nodiscard]] void* GetNativeHandle() const noexcept;
#endif
	void SetResizeCallback(std::function<void(int, int)> callback);

	std::function<void(int, int)> resizeCallback;
    bool shouldClose = false;
private:
#if defined(_WIN32)
	HWND hwnd;
	WNDCLASSW wc = { };
	static LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
#elif defined(__APPLE__)
    void* nsWindow = nullptr;
    void* view = nullptr;
    void* delegate = nullptr;
#endif
};