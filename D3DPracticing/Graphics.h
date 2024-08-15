#pragma once

#include "Window.h"
#include <d3d11.h>
#include "DxgiInfoManager.h"
#include <wrl/client.h>

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
	public:
		HrException(int line, std::string_view file, HRESULT hr, const std::vector<std::string>& messages = {});
		const char* GetType() const override;
		char const* what() const override;
		std::string GetErrorInfo() const;
	private:
		std::string info;
	};

	class DeviceRemovedException : public HrException
	{
		using HrException::HrException;
	public:
		const char* GetType() const override;
	private:
		std::string reason;
	};

	class InfoException : public BaseException
	{
	public:
		InfoException(int line, std::string_view file, const std::vector<std::string>& messages = {});
		char const* what() const override;
		const char* GetType() const override;
		std::string GetErrorInfo() const;
	private:
		std::string info;
	};

	void EndFrame();
	void ClearBuffer(float red, float green, float blue) const;
	void DrawTestTriangle(float angle, float x, float y);
private:
	Microsoft::WRL::ComPtr<ID3D11Device> pDevice;
	Microsoft::WRL::ComPtr<IDXGISwapChain> pSwap;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> pContext;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> pTarget;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> pDSV;
#ifndef NDEBUG
	DxgiInfoManager infoManager;
#endif
};
