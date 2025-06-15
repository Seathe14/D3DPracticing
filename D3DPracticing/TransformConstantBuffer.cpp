#include "TransformConstantBuffer.h"

TransformConstantBuffer::TransformConstantBuffer(Graphics& gfx, const Drawable& parent)
						: parent(parent)
{
	if(!pVcBuf)
	{
		pVcBuf = std::make_unique<VertexConstantBuffer<DirectX::XMMATRIX>>(gfx);
	}
}

void TransformConstantBuffer::Bind(Graphics& gfx)
{
	pVcBuf->Update(gfx, DirectX::XMMatrixTranspose(parent.GetTransformXM() * gfx.GetProjection()));
	pVcBuf->Bind(gfx);
}
