#include <imgui.h>
#include<iostream>
#include "Win32Window.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

Win32Window::Win32Window() {
	wc.lpfnWndProc = WndProc;
	wc.hInstance = GetModuleHandle(0);
	wc.lpszClassName = L"Wifinfo";
	RegisterClassW(&wc);

	hwnd = CreateWindowExW(0, wc.lpszClassName, L"Wifinfo", WS_OVERLAPPEDWINDOW, NULL, NULL, 1280, 720, NULL, NULL, wc.hInstance, NULL);
    SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)this);
	ShowWindow(hwnd, SW_SHOWDEFAULT);
}

Win32Window::~Win32Window() {
    DestroyWindow(hwnd);
    UnregisterClassW(wc.lpszClassName, wc.hInstance);
}

bool Win32Window::ShouldClose() {
    MSG msg;
    while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);

        if (msg.message == WM_QUIT)
            return true;
    }
    return false;
}

HWND Win32Window::GetNativeHandle() const noexcept {
    return hwnd;
}

void Win32Window::SetResizeCallback(std::function<void(int, int)> callback) {
    resizeCallback = callback;
}

LRESULT WINAPI Win32Window::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    Win32Window* window = (Win32Window*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;

    switch (msg) {
        case WM_SIZE:
            if (window && window->resizeCallback && wParam != SIZE_MINIMIZED)
                window->resizeCallback(LOWORD(lParam), HIWORD(lParam));
            return 0;
        case WM_DESTROY:
		    PostQuitMessage(0);
            return 0;
    }
    return DefWindowProcW(hWnd, msg, wParam, lParam);
}