#include "VertexShader.h"

#include <d3dcompiler.h>

#include "GraphicsMacros.h"

VertexShader::VertexShader(Graphics& gfx, const std::wstring& path)
{
	INFOMAN(gfx);

	GFX_THROW_INFO(D3DReadFileToBlob(path.c_str(), &pBytecodeBlob));
	GFX_THROW_INFO(GetDevice(gfx)->CreateVertexShader(pBytecodeBlob->GetBufferPointer(), pBytecodeBlob->GetBufferSize(), nullptr, &pVertexShader));
}

void VertexShader::Bind(Graphics& gfx)
{
	GetContext(gfx)->VSSetShader(pVertexShader.Get(), nullptr, 0);
}

ID3DBlob* VertexShader::GetBytecode() const
{
	return pBytecodeBlob.Get();
}
