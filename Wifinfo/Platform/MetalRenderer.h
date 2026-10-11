#pragma once

#include "Renderer.h"

class MetalRenderer : public Renderer {
public:
    MetalRenderer(void* nsView);
    ~MetalRenderer() override;

    void BeginFrame() override;
    void EndFrame() override;
    void OnResize(int width, int height) override;

private:
    void* view = nullptr;
    void* device = nullptr;
    void* commandQueue = nullptr;

    float clearColor[4] = {0.1f, 0.1f, 0.1f, 1.0f};
};