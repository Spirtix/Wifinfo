#pragma once
#include <functional>

#include "Window.h"

class MacWindow : public Window {
public:
    MacWindow();
    ~MacWindow() override;

    [[nodiscard]] bool ShouldClose() override;
    [[nodiscard]] NativeHandle GetNativeHandle() const noexcept override;
    void SetResizeCallback(std::function<void(int, int)> callback) override;

private:
    struct Impl;
    std::unique_ptr<Impl> impl;

    bool shouldClose = false;
    int lastWidth = 0;
    int lastHeight = 0;
    std::function<void(int, int)> resizeCallback;
};