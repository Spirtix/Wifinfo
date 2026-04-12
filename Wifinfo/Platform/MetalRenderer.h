#pragma once

#include "Renderer.h"

class MetalRenderer : public Renderer {
public:
    MetalRenderer(void* nsView);
    ~MetalRenderer();

    void BeginFrame();
    void EndFrame();
    void OnResize(int width, int height);

private:
    void* view = nullptr;
    void* device = nullptr;
    void* commandQueue = nullptr;

    float clearColor[4] = {0.1f, 0.1f, 0.1f, 1.0f};
};