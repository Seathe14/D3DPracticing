#pragma once

#define GFX_THROW_FAILED(hrCall) if (FAILED(hr = (hrCall))) throw Graphics::HrException(__LINE__, __FILE__, hr)
#define GFX_DEVICE_REMOVED_EXCEPT(hr) Graphics::DeviceRemovedException( __LINE__, __FILE__, (hr))