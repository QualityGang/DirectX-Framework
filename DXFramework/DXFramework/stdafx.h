#pragma once

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>

#include <wrl\client.h>
//#include <wrl.h>

#include <exception>

#include <d3d11.h>
#include <DirectXMath.h>

#define FREEIMAGE_LIB
#include <FreeImage.h>

#include <ft2build.h>
#include FT_FREETYPE_H

using namespace DirectX;
using namespace Microsoft::WRL;

#pragma comment(lib, "d3d11.lib")

#if _DEBUG
#pragma comment(lib, "freeimagelibd.lib")
#pragma comment(lib, "freetype255d.lib")
#else
#pragma comment(lib, "freeimagelib.lib")
#pragma comment(lib, "freetype255.lib")
#endif

#define BF(b) if (!b) throw std::exception()
#define HR(hr) if (FAILED(hr)) throw std::exception()