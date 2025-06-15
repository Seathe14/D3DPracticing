#pragma once
#include "Drawable.h"
#include "IndexBuffer.h"

template <typename T>
class DrawableBase : public Drawable
{
protected:
	static bool IsStaticInitialized() noexcept
	{
		return !staticBinds.empty();
	}

	template<BindableNotIndex C>
	static void AddStaticBind(std::unique_ptr<C> bind) noexcept(!IS_DEBUG)
	{
		staticBinds.emplace_back(std::move(bind));
	}
	void AddStaticIndexBuffer(std::unique_ptr<IndexBuffer> indexBuf) noexcept(!IS_DEBUG)
	{
		assert(pIndexBuffer == nullptr);
		pIndexBuffer = indexBuf.get();
		staticBinds.emplace_back(std::move(indexBuf));
	}
	void SetIndexBufferFromStatic() noexcept(!IS_DEBUG)
	{
		for (const auto& bind : staticBinds)
		{
			if (const auto p = dynamic_cast<IndexBuffer*>(bind.get()))
			{
				pIndexBuffer = p;
				return;
			}
		}
	}
private:
	static inline std::vector<std::unique_ptr<Bindable>> staticBinds;
	const std::vector<std::unique_ptr<Bindable>>& GetStaticBinds() const noexcept override
	{
		return staticBinds;
	}
};
