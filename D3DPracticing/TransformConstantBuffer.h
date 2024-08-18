#pragma once
#include "Bindable.h"
#include "ConstantBuffer.h"
#include <DirectXMath.h>

#include "Drawable.h"

class TransformConstantBuffer : public Bindable
{
public:
	TransformConstantBuffer(Graphics& gfx, const Drawable& parent);
	TransformConstantBuffer(const TransformConstantBuffer&) = delete;
	TransformConstantBuffer& operator=(const TransformConstantBuffer&) = delete;

	void Bind(Graphics& gfx) override;
private:
	VertexConstantBuffer<DirectX::XMMATRIX> vcBuf;
	const Drawable& parent;
};
