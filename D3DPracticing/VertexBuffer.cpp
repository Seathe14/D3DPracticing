#include "VertexBuffer.h"

void VertexBuffer::Bind(Graphics& gfx)
{
	constexpr UINT offset = 0u;
	GetContext(gfx)->IASetVertexBuffers(0u, 1u, pVertexBuffer.GetAddressOf(), &stride, &offset);
}
