#include "DxgiInfoManager.h"

#include "Window.h"
#include "WindowsMacros.h"
#include "GraphicsMacros.h"
#include "Graphics.h"

DxgiInfoManager::DxgiInfoManager()
{
	using DXGIGetDebugInterface = HRESULT(WINAPI*)(REFIID, void**);
	DxgiLibrary = LoadLibraryEx("dxgidebug.dll", nullptr, LOAD_LIBRARY_SEARCH_SYSTEM32);
	if (DxgiLibrary == nullptr)
	{
		throw WND_LAST_EXCEPT();
	}
	const auto DxgiGetDebugInterface = reinterpret_cast<DXGIGetDebugInterface>(GetProcAddress(DxgiLibrary, "DXGIGetDebugInterface"));
	if (DxgiGetDebugInterface == nullptr)
	{
		throw WND_LAST_EXCEPT();
	}
	HRESULT hr;
	GFX_THROW_NO_INFO(DxgiGetDebugInterface(__uuidof(IDXGIInfoQueue), reinterpret_cast<void**>(pDxgiInfoQueue.GetAddressOf())));
}

void DxgiInfoManager::Set()
{
	next = pDxgiInfoQueue->GetNumStoredMessages(DXGI_DEBUG_ALL);
}

std::vector<std::string> DxgiInfoManager::GetMessages() const
{
	std::vector<std::string> messages;
	const auto end = pDxgiInfoQueue->GetNumStoredMessages(DXGI_DEBUG_ALL);
	for (size_t i = next; i < end; ++i)
	{
		HRESULT hr;
		size_t messageLength{};
		GFX_THROW_NO_INFO(pDxgiInfoQueue->GetMessage(DXGI_DEBUG_ALL, i, nullptr, &messageLength));

		auto bytes = std::make_unique<byte[]>(messageLength);
		auto pMessage = reinterpret_cast<DXGI_INFO_QUEUE_MESSAGE*>(bytes.get());

		GFX_THROW_NO_INFO(pDxgiInfoQueue->GetMessage(DXGI_DEBUG_ALL, i, pMessage, &messageLength));

		messages.emplace_back(pMessage->pDescription);
	}
	return messages;
}

DxgiInfoManager::~DxgiInfoManager()
{
	if (DxgiLibrary != nullptr)
	{
		FreeLibrary(DxgiLibrary);
	}
}
