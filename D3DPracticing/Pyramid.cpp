#include "Pyramid.h"

#include "ConstantBuffer.h"
#include "PixelShader.h"
#include "VertexBuffer.h"
#include "VertexShader.h"
#include "IndexBuffer.h"
#include "InputLayout.h"
#include "Topology.h"
#include "TransformConstantBuffer.h"

Pyramid::Pyramid(Graphics& gfx, std::mt19937& rng, std::uniform_real_distribution<float>& adist,
         std::uniform_real_distribution<float>& ddist, std::uniform_real_distribution<float>& odist,
         std::uniform_real_distribution<float>& rdist)
		: r(rdist(rng))
		, droll(ddist(rng))
		, dpitch(ddist(rng))
		, dyaw(ddist(rng))
		, dphi(odist(rng))
		, dtheta(odist(rng))
		, dchi(odist(rng))
		, chi(adist(rng))
		, theta(adist(rng))
		, phi(adist(rng))
{
	struct Vertex
	{
		struct {
			float x;
			float y;
			float z;
		} pos;
	};
        const std::vector<Vertex> vertices = {
                { -1.0f, 0.0f, -1.0f },
                {  1.0f, 0.0f, -1.0f },
                {  1.0f, 0.0f,  1.0f },
                { -1.0f, 0.0f,  1.0f },
                {  0.0f, 1.5f,  0.0f },
        };
	AddBind(std::make_unique<VertexBuffer>(gfx, vertices));

	auto vertexShader = std::make_unique<VertexShader>(gfx, L"VertexShader.cso");
	auto bytecode = vertexShader->GetBytecode();
	AddBind(std::move(vertexShader));
	AddBind(std::make_unique<PixelShader>(gfx, L"PixelShader.cso"));
       const std::vector<uint16_t> indices = {
               // base
               0,1,2,
               0,2,3,
               // sides (clockwise winding when viewed from outside)
               0,4,1,
               1,4,2,
               2,4,3,
               3,4,0,

       };
	AddIndexBuffer(std::make_unique<IndexBuffer>(gfx, indices));

	struct ConstantBuffer2
	{
		struct
		{
			float r;
			float g;
			float b;
			float a;
		} face_colors[6];
	};
	const ConstantBuffer2 cb2 =
	{
		{
			{ 1.0f,0.0f,1.0f },
			{ 1.0f,0.0f,0.0f },
			{ 0.0f,1.0f,0.0f },
			{ 0.0f,0.0f,1.0f },
			{ 1.0f,1.0f,0.0f },
			{ 0.0f,1.0f,1.0f },
		}
	};
	AddBind(std::make_unique<PixelConstantBuffer<ConstantBuffer2>>(gfx, cb2));

	const std::vector<D3D11_INPUT_ELEMENT_DESC> ied = {
		{ "Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	AddBind(std::make_unique<InputLayout>(gfx, ied, bytecode));
	AddBind(std::make_unique<Topology>(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
	AddBind(std::make_unique<TransformConstantBuffer>(gfx, *this));

};

void Pyramid::Update(float dt) noexcept
{
	roll += droll * dt;
	pitch += dpitch * dt;
	yaw += dyaw * dt;
	theta += dtheta * dt;
	phi += dphi * dt;
	chi += dchi * dt;
}

DirectX::XMMATRIX Pyramid::GetTransformXM() const noexcept
{
	return DirectX::XMMatrixRotationRollPitchYaw(pitch, yaw, roll) * DirectX::XMMatrixTranslation(r, 0.0f, 0.0f) *
		DirectX::XMMatrixRotationRollPitchYaw(theta, phi, chi) * DirectX::XMMatrixTranslation(0.0f, 0.0f, 20.f);
}
