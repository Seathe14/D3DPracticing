#include "TransformConstantBuffer.h"

TransformConstantBuffer::TransformConstantBuffer(Graphics& gfx, const Drawable& parent)
						: vcBuf(gfx)
						, parent(parent)
{
}

void TransformConstantBuffer::Bind(Graphics& gfx)
{
	vcBuf.Update(gfx, DirectX::XMMatrixTranspose(parent.GetTransformXM() * gfx.GetProjection()));
	vcBuf.Bind(gfx);
}
