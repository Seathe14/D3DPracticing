#pragma once

#define GFX_EXCEPT_NO_INFO(hr) Graphics::HrException(__LINE__, __FILE__, (hr))
#define GFX_THROW_NO_INFO(hrCall) do { if (FAILED(hr = (hrCall))) throw GFX_EXCEPT_NO_INFO(hr); } while(false)

#ifndef NDEBUG
#define GFX_EXCEPT(hr) Graphics::HrException(__LINE__, __FILE__, (hr), infoManager.GetMessages())
#define GFX_THROW_INFO(hrCall) do { infoManager.Set(); if (FAILED(hr = (hrCall))) throw GFX_EXCEPT(hr); } while(false)
#define GFX_DEVICE_REMOVED_EXCEPT(hr) do { throw Graphics::DeviceRemovedException( __LINE__, __FILE__, (hr), infoManager.GetMessages()); } while(false)
#define GFX_THROW_INFO_ONLY(call) do { infoManager.Set(); (call); auto messages = infoManager.GetMessages(); if (!messages.empty()) throw Graphics::InfoException(__LINE__, __FILE__, messages); } while(false)
#else
#define GFX_EXCEPT(hr) GFX_EXCEPT_NO_INFO(hr)
#define GFX_THROW_INFO(hrCall) GFX_THROW_NO_INFO(hrCall)
#define GFX_DEVICE_REMOVED_EXCEPT(hr) do { throw Graphics::DeviceRemovedException( __LINE__,__FILE__,(hr) ); } while(false)
#define GFX_THROW_INFO_ONLY(call) do { (call); } while(false)
#endif

#ifdef NDEBUG
#define INFOMAN(gfx) HRESULT hr
#else
#define INFOMAN(gfx) HRESULT hr; DxgiInfoManager& infoManager = GetInfoManager((gfx))
#endif
