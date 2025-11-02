#pragma once
#include <d3d11.h>

class Renderer {
public:
	Renderer(HWND windowHandle);
	~Renderer();
	void BeginFrame();
	void EndFrame();

private:
	ID3D11Device* device;
	ID3D11DeviceContext* deviceContext;
	IDXGISwapChain* swapChain;
	ID3D11RenderTargetView* renderTargetView;
	const float clearColor[4] = { 0, 0, 0, 1 };

	void CreateRenderTarget();
	void ReleaseRenderTarget();
};