#pragma once
#include "Graphics.h"
#include "GraphicsMacros.h"

template<typename T>
class ConstantBuffer : public Bindable
{
public:
	ConstantBuffer(Graphics& gfx, const T& consts)
	{
		INFOMAN(gfx);
		D3D11_BUFFER_DESC constBufferDesc{};
		constBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		constBufferDesc.ByteWidth = sizeof(consts);
		constBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		constBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		constBufferDesc.MiscFlags = 0;
		constBufferDesc.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA constantBufferSbd{};
		constantBufferSbd.pSysMem = &consts;

		GFX_THROW_INFO(GetDevice(gfx)->CreateBuffer(&constBufferDesc, &constantBufferSbd, &pConstantBuffer));
		//GetContext(gfx)->VSSetConstantBuffers(0, 1, pConstantBuffer.GetAddressOf());
	}
	ConstantBuffer(Graphics& gfx)
	{
		INFOMAN(gfx);
		D3D11_BUFFER_DESC constBufferDesc{};
		constBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		constBufferDesc.ByteWidth = sizeof(T);
		constBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		constBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		constBufferDesc.MiscFlags = 0;
		constBufferDesc.StructureByteStride = 0;

		GFX_THROW_INFO(GetDevice(gfx)->CreateBuffer(&constBufferDesc, nullptr, &pConstantBuffer));
	}
	void Update(Graphics& gfx, const T& consts)
	{
		INFOMAN(gfx);
		D3D11_MAPPED_SUBRESOURCE msr{};
		GFX_THROW_INFO(GetContext(gfx)->Map(pConstantBuffer.Get(), 0u, D3D11_MAP_WRITE_DISCARD, 0u, &msr));
		memcpy(msr.pData, &consts, sizeof(consts));
		GetContext(gfx)->Unmap(pConstantBuffer.Get(), 0u);
	}
protected:
	Microsoft::WRL::ComPtr<ID3D11Buffer> pConstantBuffer;
};

template<typename T>
class PixelConstantBuffer : public ConstantBuffer<T>
{
	using Bindable::GetContext;
	using ConstantBuffer<T>::pConstantBuffer;
public:
	using ConstantBuffer<T>::ConstantBuffer;
	void Bind(Graphics& gfx) override
	{
		GetContext(gfx)->PSSetConstantBuffers(0, 1, pConstantBuffer.GetAddressOf());
	}
};

template<typename T>
class VertexConstantBuffer : public ConstantBuffer<T>
{
	using Bindable::GetContext;
	using ConstantBuffer<T>::pConstantBuffer;
public:
	using ConstantBuffer<T>::ConstantBuffer;
	void Bind(Graphics& gfx) override
	{
		GetContext(gfx)->VSSetConstantBuffers(0, 1, pConstantBuffer.GetAddressOf());
	}
};