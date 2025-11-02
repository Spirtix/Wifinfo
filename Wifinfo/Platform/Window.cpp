#include "Window.h"

Window::Window() {
	wc.lpfnWndProc = WndProc;
	wc.hInstance = GetModuleHandle(0);
	wc.lpszClassName = L"Wifinfo";
	RegisterClassW(&wc);

	hwnd = CreateWindowExW(0, wc.lpszClassName, L"Wifinfo", WS_OVERLAPPEDWINDOW, NULL, NULL, 1280, 720, NULL, NULL, wc.hInstance, NULL);
	ShowWindow(hwnd, SW_SHOWDEFAULT);
}

Window::~Window() {
    DestroyWindow(hwnd);
}

bool Window::ShouldClose() {
    MSG msg;
    while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);

        if (msg.message == WM_QUIT)
            return true;
    }
    return false;
}

HWND Window::GetNativeHandle() {
    return hwnd;
}

LRESULT WINAPI Window::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg) {
        case WM_DESTROY:
		    PostQuitMessage(0);
            return 0;
    }
    return DefWindowProcW(hWnd, msg, wParam, lParam);
}