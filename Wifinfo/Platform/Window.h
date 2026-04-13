#pragma once
#include <functional>

#include "Platform.h"

class Window {
public:
	virtual ~Window() = default;
	virtual bool ShouldClose() = 0;
	virtual NativeHandle GetNativeHandle() const noexcept = 0;
	virtual void SetResizeCallback(std::function<void(int, int)> callback) = 0;
};