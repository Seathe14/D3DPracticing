#include "Bindable.h"
#include "Graphics.h"
#include <stdexcept>

Bindable::~Bindable() = default;

ID3D11DeviceContext* Bindable::GetContext(Graphics& gfx)
{
	return gfx.pContext.Get();
}

ID3D11Device* Bindable::GetDevice(Graphics& gfx)
{
	return gfx.pDevice.Get();
}

DxgiInfoManager& Bindable::GetInfoManager(Graphics& gfx)
{
#ifndef NDEBUG
	return gfx.infoManager;
#else
	throw std::logic_error("Tried to access gfx.infoManager in Release config)");
#endif
}
