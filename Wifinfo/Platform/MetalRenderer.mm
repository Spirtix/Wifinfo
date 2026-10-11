#include <backends/imgui_impl_metal.h>
#include <backends/imgui_impl_osx.h>
#include <implot.h>

#include <stdexcept>
#include <string>

#import <Metal/Metal.h>
#import <MetalKit/MetalKit.h>

#include "MetalRenderer.h"

@interface MetalViewDelegate : NSObject<MTKViewDelegate>
@end

@implementation MetalViewDelegate
@end

// ----------------------------------------

MetalRenderer::MetalRenderer(void* nsView) {
    MTKView* mtkView = (__bridge MTKView*)nsView;
    id<MTLDevice> device = MTLCreateSystemDefaultDevice();
    if (!device) throw std::runtime_error("Metal is not supported");

    mtkView.device = device;
    mtkView.colorPixelFormat = MTLPixelFormatBGRA8Unorm;

    view = (__bridge void*)mtkView;
    commandQueue = (__bridge void*)[device newCommandQueue];

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImPlot::CreateContext();
    ImGui_ImplOSX_Init(mtkView);
    ImGui_ImplMetal_Init(device);
}

MetalRenderer::~MetalRenderer() {
    ImGui_ImplMetal_Shutdown();
    ImGui_ImplOSX_Shutdown();
    ImPlot::DestroyContext();
    ImGui::DestroyContext();
}

void MetalRenderer::BeginFrame() {
    MTKView* mtkView = (__bridge MTKView*)view;

    MTLRenderPassDescriptor* pass = mtkView.currentRenderPassDescriptor;
    if (!pass)
        return;

    ImGui_ImplMetal_NewFrame(pass);
    ImGui_ImplOSX_NewFrame(mtkView);
    ImGui::NewFrame();
}

void MetalRenderer::EndFrame() {
    ImGui::Render();
    MTKView* mtkView = (__bridge MTKView*)view;
    id<CAMetalDrawable> drawable = mtkView.currentDrawable;
    if (!drawable) return;
    MTLRenderPassDescriptor* passDesc = mtkView.currentRenderPassDescriptor;
    if (!passDesc) return;

    passDesc.colorAttachments[0].clearColor = MTLClearColorMake(
        clearColor[0], clearColor[1], clearColor[2], clearColor[3]);

    id<MTLCommandBuffer> commandBuffer = [(__bridge id<MTLCommandQueue>)commandQueue commandBuffer];
    id<MTLRenderCommandEncoder> encoder =
        [commandBuffer renderCommandEncoderWithDescriptor:passDesc];
    ImGui_ImplMetal_RenderDrawData(ImGui::GetDrawData(), commandBuffer, encoder);
    [encoder endEncoding];
    [commandBuffer presentDrawable:drawable];
    [commandBuffer commit];
}

void MetalRenderer::OnResize(int width, int height) {}
