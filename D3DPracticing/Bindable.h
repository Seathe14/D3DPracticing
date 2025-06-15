#pragma once

#include "Graphics.h"

class Bindable
{
public:
	virtual void Bind(Graphics& gfx) = 0;
	virtual ~Bindable();

protected:
	Bindable() = default;
	Bindable(const Bindable&) = delete;
	Bindable& operator=(const Bindable&) = delete;

	static ID3D11DeviceContext* GetContext(Graphics& gfx) noexcept;
	static ID3D11Device* GetDevice(Graphics& gfx) noexcept;
	static DxgiInfoManager& GetInfoManager(Graphics& gfx) noexcept(IS_DEBUG);
};