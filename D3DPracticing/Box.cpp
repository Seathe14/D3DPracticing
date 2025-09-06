#include "Box.h"

#include "ConstantBuffer.h"
#include "PixelShader.h"
#include "VertexBuffer.h"
#include "VertexShader.h"
#include "IndexBuffer.h"
#include "InputLayout.h"
#include "Topology.h"
#include "TransformConstantBuffer.h"

Box::Box(Graphics& gfx, std::mt19937& rng, std::uniform_real_distribution<float>& adist,
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
	if (!IsStaticInitialized()) {
		struct Vertex
		{
			struct {
				float x;
				float y;
				float z;
			} pos;
		};
		const std::vector<Vertex> vertices = {
			{1.0, 1.0, 1.0  },
			{1.0, 1.0, -1.0 },
			{1.0, -1.0, 1.0  },
			{1.0, -1.0, -1.0 },
			{-1.0, 1.0, 1.0  },
			{-1.0 ,1.0, -1.0 },
			{-1.0, -1.0, -1.0 },
			{-1.0, -1.0, 1.0  },
		};
		AddStaticBind(std::make_unique<VertexBuffer>(gfx, vertices));

		auto vertexShader = std::make_unique<VertexShader>(gfx, L"ShaderOutput/VertexShader.cso");
		auto bytecode = vertexShader->GetBytecode();
		AddStaticBind(std::move(vertexShader));
		AddStaticBind(std::make_unique<PixelShader>(gfx, L"ShaderOutput/PixelShader.cso"));
		const std::vector<uint16_t> indices = {
			3, 1, 0, //right
			0, 2, 3, //right
			5, 4, 0, //up
			0, 1, 5, //up
			7, 4, 5, //left
			5, 6, 7, //left
			6, 5, 1, //front
			1, 3, 6, //front
			7, 6, 3, //down
			3, 2, 7, //down
			2, 0, 4, //back
			4, 7, 2, //back

		};
		AddStaticIndexBuffer(std::make_unique<IndexBuffer>(gfx, indices));

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
		AddStaticBind(std::make_unique<PixelConstantBuffer<ConstantBuffer2>>(gfx, cb2));

		const std::vector<D3D11_INPUT_ELEMENT_DESC> ied = {
			{ "Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};
		AddStaticBind(std::make_unique<InputLayout>(gfx, ied, bytecode));
		AddStaticBind(std::make_unique<Topology>(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
	}
	else
	{
		SetIndexBufferFromStatic();
	}
	AddBind(std::make_unique<TransformConstantBuffer>(gfx, *this));

};

void Box::Update(float dt) noexcept
{
	roll += droll * dt;
	pitch += dpitch * dt;
	yaw += dyaw * dt;
	theta += dtheta * dt;
	phi += dphi * dt;
	chi += dchi * dt;
}

DirectX::XMMATRIX Box::GetTransformXM() const noexcept
{
	return DirectX::XMMatrixRotationRollPitchYaw(pitch, yaw, roll) * DirectX::XMMatrixTranslation(r, 0.0f, 0.0f) *
		DirectX::XMMatrixRotationRollPitchYaw(theta, phi, chi) * DirectX::XMMatrixTranslation(0.0f, 0.0f, 20.f);
}
