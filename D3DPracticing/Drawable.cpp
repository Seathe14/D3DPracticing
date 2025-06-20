#include "Drawable.h"

#include <cassert>

#include "IndexBuffer.h"

Drawable::~Drawable() = default;

void Drawable::Draw(Graphics& gfx) noexcept(!IS_DEBUG)
{
	for (auto& bindable : bindables)
	{
		bindable->Bind(gfx);
	}
	for (auto& bindable : GetStaticBinds())
	{
		bindable->Bind(gfx);
	}
	gfx.DrawIndexed(pIndexBuffer->GetCount());
}

void Drawable::AddIndexBuffer(std::unique_ptr<IndexBuffer> indexBuf) noexcept
{
	assert("Attempting to add index buffer a second time" && pIndexBuffer == nullptr);

	pIndexBuffer = indexBuf.get();
	bindables.emplace_back(reinterpret_cast<Bindable*>(indexBuf.release()));
}
