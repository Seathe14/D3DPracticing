#pragma once
#include "Bindable.h"

class VertexShader : public Bindable
{
public:
	VertexShader(Graphics& gfx, const std::wstring& path);
	VertexShader(const VertexShader&) = delete;
	VertexShader& operator=(const VertexShader&) = delete;

	void Bind(Graphics& gfx) override;
	ID3DBlob* GetBytecode() const;

protected:
	Microsoft::WRL::ComPtr<ID3DBlob> pBytecodeBlob;
	Microsoft::WRL::ComPtr<ID3D11VertexShader> pVertexShader;
};

