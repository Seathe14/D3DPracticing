#pragma once
#include "Bindable.h"
#include "GraphicsMacros.h"

class VertexBuffer : public Bindable
{
public:
	template<typename T>
	VertexBuffer(Graphics& gfx, const std::vector<T>& vertices)
		: stride(sizeof(T))
	{
		INFOMAN(gfx);
		//initialize vertex buffer
		D3D11_BUFFER_DESC bufferDesc;
		bufferDesc.Usage = D3D11_USAGE_DEFAULT;
		bufferDesc.ByteWidth = stride * static_cast<UINT>(vertices.size());
		bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bufferDesc.CPUAccessFlags = 0;
		bufferDesc.MiscFlags = 0;
		bufferDesc.StructureByteStride = stride;

		D3D11_SUBRESOURCE_DATA initData;
		initData.pSysMem = vertices.data();
		initData.SysMemPitch = 0;
		initData.SysMemSlicePitch = 0;

		GFX_THROW_INFO(GetDevice(gfx)->CreateBuffer(&bufferDesc, &initData, &pVertexBuffer));
	}
	VertexBuffer(const VertexBuffer&) = delete;
	VertexBuffer& operator=(const VertexBuffer&) = delete;

	void Bind(Graphics& gfx) override;
protected:
	UINT stride {0};
	Microsoft::WRL::ComPtr<ID3D11Buffer> pVertexBuffer;
};
