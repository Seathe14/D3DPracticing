#include "Graphics.h"

#include <ostream>
#include <sstream>
#include <dxdiag.h>
#include "GraphicsMacros.h"

Graphics::Graphics(HWND hWnd)
{
	DXGI_SWAP_CHAIN_DESC sd = {};
	sd.BufferDesc.Width = 0;
	sd.BufferDesc.Height = 0;
	sd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 0;
	sd.BufferDesc.RefreshRate.Denominator = 0;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount = 1;
	sd.OutputWindow = hWnd;
	sd.Windowed = true;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	sd.Flags = 0;

	HRESULT hr;
	GFX_THROW_FAILED(D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		0,
		nullptr,
		0,
		D3D11_SDK_VERSION,
		&sd,
		&pSwap,
		&pDevice,
		nullptr,
		&pContext
	));
	ID3D11Resource* pBackBuffer = nullptr;
	pSwap->GetBuffer(0, _uuidof(ID3D11Resource), reinterpret_cast<void**>(&pBackBuffer));
	pDevice->CreateRenderTargetView(pBackBuffer, nullptr, &pTarget);
	pBackBuffer->Release();
}

Graphics::~Graphics()
{
	if (pDevice != nullptr) {
		pDevice->Release();
	}
	if (pSwap != nullptr) {
		pSwap->Release();
	}
	if (pContext != nullptr) {
		pContext->Release();
	}
	if (!pTarget)
	{
		pTarget->Release();
	}
}

const char* Graphics::HrException::GetType() const
{
	return "Graphics Exception";
}

const char* Graphics::DeviceRemovedException::GetType() const
{
	return "Graphics Exception [Device Removed]";
}

void Graphics::EndFrame()
{
	pSwap->Present(1u, 0u);
}

void Graphics::ClearBuffer(float red, float green, float blue)
{
	const float color[] = { red, green, blue };
	pContext->ClearRenderTargetView(pTarget, color);
}
