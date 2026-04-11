#include <backends/imgui_impl_dx11.h>
#include <backends/imgui_impl_win32.h>
#include <implot.h>
#include <stdexcept>
#include <string>
#include <format>
#include <comdef.h>

#include "DirectXRenderer.h"

using Microsoft::WRL::ComPtr;

DirectXRenderer::DirectXRenderer(HWND windowHandle) {
	DXGI_SWAP_CHAIN_DESC sd = {};
    sd.BufferCount = 2;
    sd.BufferDesc.Width = 0;
    sd.BufferDesc.Height = 0;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = windowHandle;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = TRUE;
    sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    UINT createDeviceFlags = 0;
    D3D_FEATURE_LEVEL featureLevel;
    const D3D_FEATURE_LEVEL featureLevelArray[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };
    HRESULT res = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, swapChain.GetAddressOf(), device.GetAddressOf(), &featureLevel, deviceContext.GetAddressOf());
    if (res == DXGI_ERROR_UNSUPPORTED)
        res = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_WARP, nullptr, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, swapChain.GetAddressOf(), device.GetAddressOf(), &featureLevel, deviceContext.GetAddressOf());
    
    ThrowIfFailed(res, "DirecX initialization failed: ");

    CreateRenderTarget();

    ImGui::CreateContext();
    ImPlot::CreateContext();
    ImGui_ImplWin32_Init(windowHandle);
    ImGui_ImplDX11_Init(device.Get(), deviceContext.Get());
}

DirectXRenderer::~DirectXRenderer() {
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImPlot::DestroyContext();
    ImGui::DestroyContext();
    ReleaseRenderTarget();
}

void DirectXRenderer::BeginFrame() {
    ImGui_ImplDX11_NewFrame(); // no need to call RSSetViewport because ImGUI does it
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
}

void DirectXRenderer::EndFrame() {
    ImGui::Render();
    deviceContext->OMSetRenderTargets(1, renderTargetView.GetAddressOf(), nullptr);
    deviceContext->ClearRenderTargetView(renderTargetView.Get(), DirectXRenderer::clearColor);
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
    swapChain->Present(1, 0);
}

void DirectXRenderer::OnResize(int width, int height) {
    if (!device)
        return;
    
    ReleaseRenderTarget();
    swapChain->ResizeBuffers(0, width, height, DXGI_FORMAT_UNKNOWN, 0);
    CreateRenderTarget();
}

void DirectXRenderer::CreateRenderTarget() {
    ComPtr<ID3D11Texture2D> backBuffer;
    HRESULT hr = swapChain->GetBuffer(0, IID_PPV_ARGS(backBuffer.GetAddressOf()));
    ThrowIfFailed(hr, "DirectX failed to get BackBuffer: ");
    hr = device->CreateRenderTargetView(backBuffer.Get(), nullptr, renderTargetView.ReleaseAndGetAddressOf());
    ThrowIfFailed(hr, "DirectX failed to create RTV: ");
}

void DirectXRenderer::ReleaseRenderTarget() {
    renderTargetView.Reset();
}

void DirectXRenderer::ThrowIfFailed(HRESULT hr, const std::string& message) {
    if (FAILED(hr)) {
        _com_error err(hr);
        throw std::runtime_error(std::format("{} ({}) {}", message, hr, err.ErrorMessage()));
    }
}
