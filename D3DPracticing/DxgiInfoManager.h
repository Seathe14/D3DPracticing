#pragma once
#include <string>
#include <vector>
#include <wrl/client.h>
#include <dxgidebug.h>

class DxgiInfoManager
{
public:
	DxgiInfoManager();
	DxgiInfoManager(const DxgiInfoManager&) = delete;
	DxgiInfoManager& operator=(const DxgiInfoManager&) = delete;
	~DxgiInfoManager();

	void Set();

	std::vector<std::string> GetMessages() const;

private:
	uint64_t next = 0u;
	Microsoft::WRL::ComPtr<IDXGIInfoQueue> pDxgiInfoQueue;
	HMODULE DxgiLibrary{};
};