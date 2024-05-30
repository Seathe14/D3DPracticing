#pragma once

#define GFX_EXCEPT_NO_INFO(hr) Graphics::HrException(__LINE__, __FILE__, (hr))
#define GFX_THROW_NO_INFO(hrCall) if (FAILED(hr = (hrCall))) throw GFX_EXCEPT_NO_INFO(hr)

#ifndef NDEBUG
#define GFX_EXCEPT(hr) Graphics::HrException(__LINE__, __FILE__, (hr), infoManager.GetMessages())
#define GFX_THROW_INFO(hrCall) infoManager.Set(); if (FAILED(hr = (hrCall))) throw GFX_EXCEPT(hr)
#define GFX_DEVICE_REMOVED_EXCEPT(hr) Graphics::DeviceRemovedException( __LINE__, __FILE__, (hr), infoManager.GetMessages())
#else
#define GFX_EXCEPT(hr) GFX_EXCEPT_NO_INFO(hr)
#define GFX_THROW_INFO(hrcall) GFX_THROW_NO_INFO(hrCall)
#define GFX_DEVICE_REMOVED_EXCEPT(hr) Graphics::DeviceRemovedException( __LINE__,__FILE__,(hr) )
#endif