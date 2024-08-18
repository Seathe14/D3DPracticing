#include "IndexBuffer.h"

#include "GraphicsMacros.h"

using namespace Microsoft::WRL;

IndexBuffer::IndexBuffer(Graphics& gfx, const std::vector<uint16_t>& indices)
	: count(static_cast<UINT>(indices.size()))
{
	INFOMAN(gfx);

	D3D11_BUFFER_DESC ibd{};
	//Intiialize index buffer
	D3D11_BUFFER_DESC indexBufferDesc;
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = count * sizeof(uint16_t);
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = sizeof(uint16_t);

	D3D11_SUBRESOURCE_DATA indexBufferSbd{};
	indexBufferSbd.pSysMem = indices.data();
	indexBufferSbd.SysMemPitch = 0;
	indexBufferSbd.SysMemSlicePitch = 0;

	GFX_THROW_INFO(GetDevice(gfx)->CreateBuffer(&indexBufferDesc, &indexBufferSbd, &pIndexBuffer));
}

void IndexBuffer::Bind(Graphics& gfx)
{
	GetContext(gfx)->IASetIndexBuffer(pIndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);
}
