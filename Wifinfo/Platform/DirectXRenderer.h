#pragma once
#include <d3d11.h>
#include <wrl/client.h>

#include "Renderer.h"

class DirectXRenderer : public Renderer {
public:
	DirectXRenderer(HWND windowHandle);
	~DirectXRenderer() override;
	void BeginFrame() override;
	void EndFrame() override;
	void OnResize(int width, int height) override;

private:
	Microsoft::WRL::ComPtr<ID3D11Device> device;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> deviceContext;
	Microsoft::WRL::ComPtr<IDXGISwapChain> swapChain;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> renderTargetView;
	const float clearColor[4] = { 0, 0, 0, 1 };

	void CreateRenderTarget();
	void ReleaseRenderTarget();
	static void ThrowIfFailed(HRESULT hr, const std::string& message);
};