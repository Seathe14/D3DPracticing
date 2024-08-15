#include "Graphics.h"

#include <ostream>
#include <sstream>
#include <dxdiag.h>
#include "GraphicsMacros.h"
#include <d3dcompiler.h>
#include <DirectXMath.h>

using Microsoft::WRL::ComPtr;
using namespace DirectX;

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

	D3D11_DEPTH_STENCIL_DESC dsDesc{};
	dsDesc.DepthEnable = TRUE;
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsDesc.DepthFunc = D3D11_COMPARISON_LESS;
	ComPtr<ID3D11DepthStencilState> pDSState;
	GFX_THROW_INFO(pDevice->CreateDepthStencilState(&dsDesc, &pDSState));
	pContext->OMSetDepthStencilState(pDSState.Get(), 1u);

	ComPtr<ID3D11Texture2D> pDepthStencil;
	D3D11_TEXTURE2D_DESC descDepth{};
	descDepth.Width = 800u;
	descDepth.Height = 600u;
	descDepth.MipLevels = 1u;
	descDepth.ArraySize = 1u;
	descDepth.Format = DXGI_FORMAT_D32_FLOAT;
	descDepth.SampleDesc.Count = 1u;
	descDepth.SampleDesc.Quality = 0u;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;

	GFX_THROW_INFO(pDevice->CreateTexture2D(&descDepth, nullptr, &pDepthStencil));
	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV{};
	descDSV.Format = DXGI_FORMAT_D32_FLOAT;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0u;
	GFX_THROW_INFO(pDevice->CreateDepthStencilView(pDepthStencil.Get(), &descDSV, &pDSV));
	//pContext->OMSetRenderTargets(1u, pTarget.GetAddressOf(), pDSV.Get());
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

void Graphics::ClearBuffer(float red, float green, float blue) const
{
	const float color[] = { red, green, blue };
	pContext->ClearRenderTargetView(pTarget.Get(), color);
	pContext->ClearDepthStencilView(pDSV.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0u);
}

void Graphics::DrawTestTriangle(float angle, float x, float y)
{
	struct Vertex
	{
		struct {
			float x;
			float y;
			float z;
		} pos;
	};
	constexpr Vertex vertices[] = {
		{1.0, 1.0 , 1.0  },//, 255, 0, 0, 255}, //0
		{1.0, 1.0 , -1.0 },//, 0, 255,0,255}, //1
		{1.0,-1.0 ,1.0   },//,0,255,0,255}, //2
		{1.0,-1.0 ,-1.0  },//,0,0,255,255}, //3
		{-1.0,1.0 ,1.0   },//,0,0,255,255}, //4
		{-1.0,1.0 ,-1.0  },//, 255,0,0,255}, //5
		{-1.0,-1.0,-1.0  },//,255,0,0,255}, //6
		{-1.0,-1.0,1.0   },//,0,255,0,255} //7
	};

	constexpr Vertex vertices2[] = {
		{0.25, 0.25 },//, 0.25, 255, 0, 0, 255}, //0
		{0.25, 0.25 },//, -0.25, 255,0,0,255}, //1
		{0.25,-0.25 },//,0.25,255,0,0,255}, //2
		{0.25,-0.25 },//,-0.25,255,0,0,255}, //3
		{-0.25,0.25 },//,0.25,0,255,0,255}, //4
		{-0.25,0.25 },//,-0.25, 0,255,0,255}, //5
		{-0.25,-0.25},//,-0.25,0,0,255,255}, //6
		{-0.25,-0.25},//,0.25,0,0,255,255} //7
	};

	constexpr uint16_t indices[] = {
		3, 1, 0, //right
		0, 2, 3, //right
		5, 4, 0, //up
		0, 1, 5, //up
		7, 4, 5, //left
		5, 6, 7, //left
		6, 5, 1, //front
		1, 3, 6, //front
		7, 6, 3, //down
		3, 2, 7, //down
		2, 0, 4, //back
		4, 7, 2, //back

	};
	constexpr D3D11_INPUT_ELEMENT_DESC layout[] = {
		{ "Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	HRESULT hr;

	//Intiialize index buffer
	D3D11_BUFFER_DESC indexBufferDesc;
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(indices);
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = sizeof(uint16_t);

	D3D11_SUBRESOURCE_DATA indexBufferSbd;
	indexBufferSbd.pSysMem = indices;
	indexBufferSbd.SysMemPitch = 0;
	indexBufferSbd.SysMemSlicePitch = 0;

	ComPtr<ID3D11Buffer> pIndexBuffer;
	GFX_THROW_INFO(pDevice->CreateBuffer(&indexBufferDesc, &indexBufferSbd, &pIndexBuffer));

	pContext->IASetIndexBuffer(pIndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);
	//end
	
	//initialize vertex buffer
	D3D11_BUFFER_DESC bufferDesc;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = sizeof(vertices);
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = sizeof(Vertex);

	D3D11_SUBRESOURCE_DATA initData;
	initData.pSysMem = vertices;
	initData.SysMemPitch = 0;
	initData.SysMemSlicePitch = 0;

	ComPtr<ID3D11Buffer> pBuff;
	GFX_THROW_INFO(pDevice->CreateBuffer(&bufferDesc, &initData, &pBuff));
	constexpr UINT stride = sizeof(Vertex);
	constexpr UINT offset = 0;
	pContext->IASetVertexBuffers(0, 1, pBuff.GetAddressOf(), &stride, &offset);
	//end

	//
	struct ConstantBuffer
	{
		XMMATRIX transformation;
	};

	const ConstantBuffer cb = {
		{
			XMMatrixTranspose(XMMatrixRotationZ(angle) * XMMatrixRotationX(angle) * XMMatrixTranslation(x, 0, y + 3.f)
							  * XMMatrixPerspectiveLH(1.0f, 3.0f / 4.0f, 0.5f, 10.0f))
		}
	};

	D3D11_BUFFER_DESC constBufferDesc{};
	constBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	constBufferDesc.ByteWidth = sizeof(cb);
	constBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	constBufferDesc.MiscFlags = 0;
	constBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA constantBufferSbd;
	constantBufferSbd.pSysMem = &cb;

	ComPtr<ID3D11Buffer> pConstantBuffer;
	GFX_THROW_INFO(pDevice->CreateBuffer(&constBufferDesc, &constantBufferSbd, &pConstantBuffer));
	pContext->VSSetConstantBuffers(0, 1, pConstantBuffer.GetAddressOf());
	//

	//bind vertex shader
	ComPtr<ID3DBlob> pBlob;
	D3DReadFileToBlob(L"VertexShader.cso", &pBlob);

	pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	ComPtr<ID3D11InputLayout> pLayout;
	GFX_THROW_INFO(pDevice->CreateInputLayout(layout, 1, pBlob->GetBufferPointer(), pBlob->GetBufferSize(), &pLayout));
	pContext->IASetInputLayout(pLayout.Get());

	ComPtr<ID3D11VertexShader> pVertexShader;
	GFX_THROW_INFO(pDevice->CreateVertexShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pVertexShader));
	pContext->VSSetShader(pVertexShader.Get(), nullptr, 0);
	//end

	//constant buffer for pixel shader
	struct PixelConstantBuffer
	{
		float r;
		float g;
		float b;
		std::byte padding[4]{};
	};
	PixelConstantBuffer pCb[6] = 
	{
		{1.0f, 0.0f, 1.0f},
		{1.0f,0.0f,0.0f},
		{0.0f,1.0f,0.0f},
		{0.0f,0.0f,1.0f},
		{0.0f,1.0f,1.0f},
		{1.0f,1.0f,0.0f}
	};

	constBufferDesc.ByteWidth = sizeof(pCb);

	constantBufferSbd.pSysMem = &pCb;

	//ComPtr<ID3D11Buffer> pConstantBuffer;
	GFX_THROW_INFO(pDevice->CreateBuffer(&constBufferDesc, &constantBufferSbd, &pConstantBuffer));
	pContext->PSSetConstantBuffers(0, 1, pConstantBuffer.GetAddressOf());

	//end

	//bind pixel shader
	D3DReadFileToBlob(L"PixelShader.cso", &pBlob);
	ComPtr<ID3D11PixelShader> pPixelShader;
	GFX_THROW_INFO(pDevice->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pPixelShader));
	pContext->PSSetShader(pPixelShader.Get(), nullptr, 0);
	//

	pContext->OMSetRenderTargets(1, pTarget.GetAddressOf(), pDSV.Get());
	D3D11_VIEWPORT viewPort;
	viewPort.TopLeftX = 0.0;
	viewPort.TopLeftY = 0.0;
	viewPort.Width = 800;
	viewPort.Height = 600;
	viewPort.MaxDepth = 1;
	viewPort.MinDepth = 0;
	pContext->RSSetViewports(1, &viewPort);
	
	GFX_THROW_INFO_ONLY(pContext->DrawIndexed(static_cast<UINT>(std::size(indices)), 0u, 0));
}
