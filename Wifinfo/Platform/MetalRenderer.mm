#include <backends/imgui_impl_metal.h>
#include <backends/imgui_impl_osx.h>
#include <implot.h>

#include <stdexcept>
#include <string>

#import <Metal/Metal.h>
#import <MetalKit/MetalKit.h>

#include "MetalRenderer.h"

struct MetalRenderer::Impl {
    MTKView* view = nil;
    id<MTLDevice> device = nil;
    id<MTLCommandQueue> commandQueue = nil;
};

MetalRenderer::MetalRenderer(void* nsView) : impl(std::make_unique<Impl>()) {
    impl->view = (MTKView*)nsView;
    impl->device = MTLCreateSystemDefaultDevice();
    if (!impl->device) throw std::runtime_error("Metal is not supported");

    impl->view.device = impl->device;
    impl->view.colorPixelFormat = MTLPixelFormatBGRA8Unorm;

    impl->commandQueue = [impl->device newCommandQueue];

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImPlot::CreateContext();
    ImGui_ImplOSX_Init(impl->view);
    ImGui_ImplMetal_Init(impl->device);
}

MetalRenderer::~MetalRenderer() {
    ImGui_ImplMetal_Shutdown();
    ImGui_ImplOSX_Shutdown();
    ImPlot::DestroyContext();
    ImGui::DestroyContext();
    
    [impl->commandQueue release];
    [impl->device release];
}

void MetalRenderer::BeginFrame() {
    @autoreleasepool {
        MTLRenderPassDescriptor* pass = impl->view.currentRenderPassDescriptor;
        if (!pass)
            return;
        
        ImGui_ImplMetal_NewFrame(pass);
        ImGui_ImplOSX_NewFrame(impl->view);
        ImGui::NewFrame();
    }
}

void MetalRenderer::EndFrame() {
    ImGui::Render();
    
    @autoreleasepool {
        id<CAMetalDrawable> drawable = impl->view.currentDrawable;
        if (!drawable) return;
        MTLRenderPassDescriptor* passDesc = impl->view.currentRenderPassDescriptor;
        if (!passDesc) return;
        
        passDesc.colorAttachments[0].clearColor = MTLClearColorMake(
                                                                    clearColor[0], clearColor[1], clearColor[2], clearColor[3]);
        
        id<MTLCommandBuffer> commandBuffer = [impl->commandQueue commandBuffer];
        id<MTLRenderCommandEncoder> encoder =
        [commandBuffer renderCommandEncoderWithDescriptor:passDesc];
        ImGui_ImplMetal_RenderDrawData(ImGui::GetDrawData(), commandBuffer, encoder);
        [encoder endEncoding];
        [commandBuffer presentDrawable:drawable];
        [commandBuffer commit];
    }
}

void MetalRenderer::OnResize(int width, int height) {}
