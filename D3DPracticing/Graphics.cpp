#include "Graphics.h"

#include <ostream>
#include <sstream>
#include <dxdiag.h>
#include "GraphicsMacros.h"
#include <d3dcompiler.h>

using Microsoft::WRL::ComPtr;

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
	sd.BufferCount = 2;
	sd.OutputWindow = hWnd;
	sd.Windowed = true;
	sd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	sd.Flags = 0;
	UINT swapCreateFlags = 0u;
#ifndef NDEBUG
	swapCreateFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
	HRESULT hr;

	GFX_THROW_INFO(D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		swapCreateFlags,
		nullptr,
		0,
		D3D11_SDK_VERSION,
		&sd,
		&pSwap,
		&pDevice,
		nullptr,
		&pContext
	));
	ComPtr<ID3D11Resource> pBackBuffer;
	GFX_THROW_INFO(pSwap->GetBuffer(0, _uuidof(ID3D11Resource), &pBackBuffer));
	GFX_THROW_INFO(pDevice->CreateRenderTargetView(pBackBuffer.Get(), nullptr, &pTarget));
}

Graphics::~Graphics() = default;

Graphics::HrException::HrException(int line, std::string_view file, HRESULT hr, const std::vector<std::string>& messages)
	: Window::Exception(line, file, hr)
{
	for (const auto& m : messages)
	{
		info += m;
		info.push_back('\n');
	}
	if (!info.empty())
	{
		info.pop_back();
	}
}

const char* Graphics::HrException::GetType() const
{
	return "Graphics Exception";
}

char const* Graphics::HrException::what() const
{
	std::ostringstream oss;
	oss << GetType() << std::endl
		<< "[Error code] 0x" << std::hex << std::uppercase << GetErrorCode()
		<< std::dec << " (" << static_cast<unsigned long>(GetErrorCode()) << ")" << std::endl
		<< "[Description] " << GetErrorString() << std::endl;
	if (!info.empty())
	{
		oss << "\n[Error Info]\n" << GetErrorInfo() << std::endl;
	}
	oss	<< GetOriginString();
	whatBuffer = oss.str();
	return whatBuffer.c_str();
}

std::string Graphics::HrException::GetErrorInfo() const
{
	return info;
}

const char* Graphics::DeviceRemovedException::GetType() const
{
	return "Graphics Exception [Device Removed]";
}

Graphics::InfoException::InfoException(int line, std::string_view file, const std::vector<std::string>& messages)
	: BaseException(line, file)
{
	for (const auto& m : messages)
	{
		info += m;
		info.push_back('\n');
	}
	if (!info.empty())
	{
		info.pop_back();
	}
}

char const* Graphics::InfoException::what() const
{
	std::ostringstream oss;
	oss << GetType() << std::endl;
	if (!info.empty())
	{
		oss << "\n[Error Info]\n" << GetErrorInfo() << std::endl;
	}
	oss << GetOriginString();
	whatBuffer = oss.str();
	return whatBuffer.c_str();
}

const char* Graphics::InfoException::GetType() const
{
	return "Graphics info exception";
}

std::string Graphics::InfoException::GetErrorInfo() const
{
	return info;
}

void Graphics::EndFrame()
{
	HRESULT hr;
#ifndef NDEBUG
	infoManager.Set();
#endif
	if (FAILED(hr = pSwap->Present(1u, 0u)))
	{
		if (hr == DXGI_ERROR_DEVICE_REMOVED)
		{
			throw GFX_DEVICE_REMOVED_EXCEPT(pDevice->GetDeviceRemovedReason());
		}
		else
		{
			throw GFX_EXCEPT(hr);
		}
	}
}

void Graphics::ClearBuffer(float red, float green, float blue)
{
	const float color[] = { red, green, blue };
	pContext->ClearRenderTargetView(pTarget.Get(), color);
}

void Graphics::DrawTestTriangle()
{
	struct Vertex
	{
		float x;
		float y;
	};
	const Vertex vertices[] = {
		{ 0.0, 0.5},
		{0.5, -0.5},
		{-0.5,-0.5}
	};
	D3D11_INPUT_ELEMENT_DESC layout[] = {
		{ "Position", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	D3D11_BUFFER_DESC bufferDesc;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = sizeof(vertices);
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA initData;
	initData.pSysMem = vertices;
	initData.SysMemPitch = 0;
	initData.SysMemSlicePitch = 0;

	ComPtr<ID3D11Buffer> pBuff;
	pDevice->CreateBuffer(&bufferDesc, &initData, &pBuff);

	ComPtr<ID3DBlob> pBlob;
	D3DReadFileToBlob(L"VertexShader.cso", &pBlob);

	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	ComPtr<ID3D11InputLayout> pLayout;
	pDevice->CreateInputLayout(layout, 1, pBlob->GetBufferPointer(), pBlob->GetBufferSize(), &pLayout);
	pContext->IASetVertexBuffers(0, 1, pBuff.GetAddressOf(), &stride, &offset);
	pContext->IASetInputLayout(pLayout.Get());
	pContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	ComPtr<ID3D11VertexShader> pVertexShader;
	pDevice->CreateVertexShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pVertexShader);
	pContext->VSSetShader(pVertexShader.Get(), nullptr, 0);

	D3DReadFileToBlob(L"PixelShader.cso", &pBlob);
	ComPtr<ID3D11PixelShader> pPixelShader;
	pDevice->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pPixelShader);
	pContext->PSSetShader(pPixelShader.Get(), nullptr, 0);

	pContext->OMSetRenderTargets(1, pTarget.GetAddressOf(), nullptr);
	D3D11_VIEWPORT viewPort;
	viewPort.TopLeftX = 0.0;
	viewPort.TopLeftY = 0.0;
	viewPort.Width = 800;
	viewPort.Height = 600;
	viewPort.MaxDepth = 0;
	viewPort.MinDepth = 0;
	pContext->RSSetViewports(1, &viewPort);
	
	GFX_THROW_INFO_ONLY(pContext->Draw(3u, 0u));
}
