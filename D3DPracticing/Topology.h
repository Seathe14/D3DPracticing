#pragma once
#include "Bindable.h"

class Topology : public Bindable
{
public:
	Topology() = default;
	Topology(const Topology&) = delete;
	Topology& operator= (const Topology&) = delete;

	Topology(D3D11_PRIMITIVE_TOPOLOGY type);
	void Bind(Graphics& gfx) override;
protected:
	D3D11_PRIMITIVE_TOPOLOGY type;
};