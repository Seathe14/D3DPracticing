#pragma once

#include "Window.h"
#include <d3d11.h>

class Graphics
{
public:
	Graphics(HWND hWnd);
	Graphics(const Graphics&) = delete;
	Graphics& operator=(const Graphics&) = delete;
	Graphics(Graphics&&) = delete;
	Graphics& operator=(Graphics&&) = delete;
	~Graphics();


	class HrException : public Window::Exception
	{
		using Exception::Exception;
	public:
		const char* GetType() const override;
	};

	class DeviceRemovedException : HrException
	{
		using HrException::HrException;
	public:
		const char* GetType() const override;
	};

	void EndFrame();
	void ClearBuffer(float red, float green, float blue);
private:
	ID3D11Device* pDevice = nullptr;
	IDXGISwapChain* pSwap = nullptr;
	ID3D11DeviceContext* pContext = nullptr;
	ID3D11RenderTargetView* pTarget = nullptr;
};
