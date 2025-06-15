#pragma once
#include <random>

#include "Drawable.h"

class Pyramid : public Drawable
{
public:
	Pyramid(Graphics& gfx, std::mt19937& rng, std::uniform_real_distribution<float>& adist,
		std::uniform_real_distribution<float>& ddist, std::uniform_real_distribution<float>& odist,
		std::uniform_real_distribution<float>& rdist);
	Pyramid(const Pyramid&) = delete;
	Pyramid& operator=(const Pyramid&) = delete;

	void Update(float dt) noexcept override;
	DirectX::XMMATRIX GetTransformXM() const noexcept override;
private:
	float r{ 0.0f };
	float roll{ 0.0f };
	float pitch{ 0.0f };
	float yaw{ 0.0f };
	float theta{ 0.0f };
	float phi{ 0.0f };
	float chi{ 0.0f };

	float droll{ 0.0f };
	float dpitch{ 0.0f };
	float dyaw{ 0.0f };
	float dtheta{ 0.0f };
	float dphi{ 0.0f };
	float dchi{ 0.0f };
};
