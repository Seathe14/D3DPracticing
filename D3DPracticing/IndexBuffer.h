#pragma once
#include "Bindable.h"

class IndexBuffer : public Bindable
{
public:
	IndexBuffer(Graphics& gfx, const std::vector<uint16_t>& indices);
	IndexBuffer(const IndexBuffer&) = delete;
	IndexBuffer& operator=(const IndexBuffer&) = delete;

	void Bind(Graphics& gfx) override;
	UINT GetCount() const noexcept { return count; }
private:
	UINT count{};
	Microsoft::WRL::ComPtr<ID3D11Buffer> pIndexBuffer;
};