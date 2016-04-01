#pragma once

// Remove unused features
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#define _COM_NO_STANDARD_GUIDS_

// Windows headers
#include <Windows.h>
#include <comdef.h>
//#include <wrl\client.h>
//#include <wrl.h>

// D3D11 headers
#include <d3d11.h>
#include <DirectXMath.h>

// Extern libs
#define FREEIMAGE_LIB
#include <FreeImage.h>

#include <ft2build.h>
#include FT_FREETYPE_H


// Utilities
#include "Logger.h"

#define PrintError(hr) do { _com_error err(hr); error_stream << err.ErrorMessage() << " " << __FILE__ << " " << __LINE__ << std::endl; } while (false)
#define SafeAddRef(ptr) do { if (ptr) { ptr->AddRef(); } } while (false)
#define SafeRelease(ptr) do { if (ptr) { ptr->Release(); ptr = nullptr; } } while (false)

#define DEPRECATED __declspec(deprecated)
#define ALIGN(a) __declspec(align(a))

using namespace DirectX;
//using namespace Microsoft::WRL;

#if VLD_ENABLED
#include <vld.h>
#endif


// Linking libs
#pragma comment(lib, "d3d11.lib")

#ifdef _DEBUG
#pragma comment(lib, "FreeImageLibd.lib")
#pragma comment(lib, "freetype255d.lib")
#else
#pragma comment(lib, "FreeImageLib.lib")
#pragma comment(lib, "freetype255.lib")
#endif

