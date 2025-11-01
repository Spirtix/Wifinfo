#pragma once
#include <Windows.h>

class Window {
public:
	Window();
	~Window();
	bool ShouldClose();
private:
	HWND hwnd;
	WNDCLASSW wc = { };
	static LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
};