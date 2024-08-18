#pragma once
#include <DirectXMath.h>

#include "Bindable.h"
#include "Graphics.h"
#include <vector>
#include <type_traits>

class IndexBuffer;

template<typename T>
concept BindableNotIndex = std::is_base_of_v<Bindable, T> && !std::is_same_v<T, IndexBuffer>;

class Drawable
{
public:
	Drawable() = default;
	Drawable(const Drawable&) = delete;
	Drawable& operator=(const Drawable&) = delete;
	virtual ~Drawable();

	virtual void Update(float dt) noexcept = 0;
	virtual DirectX::XMMATRIX GetTransformXM() const noexcept = 0;

	void Draw(Graphics& gfx) noexcept(!IS_DEBUG);

	template<BindableNotIndex T>
	void AddBind(std::unique_ptr<T> bind) noexcept(!IS_DEBUG)
	{
		
		bindables.emplace_back(std::move(bind));
	}

	void AddIndexBuffer(std::unique_ptr<IndexBuffer> ibuf) noexcept;
private:
	const IndexBuffer* pIndexBuffer = nullptr;
	std::vector<std::unique_ptr<Bindable>> bindables;
};