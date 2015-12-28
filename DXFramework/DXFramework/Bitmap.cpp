#include "stdafx.h"
#include "Bitmap.h"
#include "Pipeline.h"

Bitmap::Bitmap(UINT width, UINT height, const BYTE* srcPixels) :
	size({ width,height }),
	pixels(new BYTE[width * height * 4])
{
	UINT length = width * height * 4;

	BF(length > 0);

	if (srcPixels)
		memcpy(pixels, srcPixels, length);
	else
		ZeroMemory(pixels, length);
}

Bitmap::Bitmap(UINT width, UINT height, BYTE r, BYTE g, BYTE b, BYTE a) :
	size({ width,height }),
	pixels(new BYTE[width * height * 4])
{
	UINT length = width * height * 4;

	BF(length > 0);

	for (UINT y = 0; y < height; ++y)
	{
		for (UINT x = 0; x < width; ++x)
		{
			setPixel(x, y, r, g, b, a);
		}
	}
}

Bitmap::Bitmap(const Bitmap& other)
{
	*this = other;
}

Bitmap Bitmap::operator=(const Bitmap& other)
{
	if (this != &other)
	{
		size = other.size;

		delete[] pixels;

		UINT length = size.x * size.y * 4;
		pixels = new BYTE[length];
		memcpy(pixels, other.pixels, length);
	}

	return *this;
}

Bitmap::~Bitmap()
{
	delete[] pixels;
}

void Bitmap::setPixel(UINT x, UINT y, BYTE r, BYTE g, BYTE b, BYTE a)
{
	BF(x >= 0 && y >= 0);
	BF(x + 1 < size.x && y + 1 < size.y);

	setPixel(y * size.x + x, r, g, b, a);
}

void Bitmap::setPixel(UINT index, BYTE r, BYTE g, BYTE b, BYTE a)
{
	BF(index >= 0 && index < (size.x * size.y * 4));

	pixels[index * 4] = r;
	pixels[index * 4 + 1] = g;
	pixels[index * 4 + 2] = b;
	pixels[index * 4 + 3] = a;
}

void Bitmap::setPixelRect(UINT x, UINT y, UINT width, UINT height, BYTE r, BYTE g, BYTE b, BYTE a)
{
	BF(x >= 0 && y >= 0);
	BF(x < size.x && y < size.y);

	for (UINT iy = y; y < height; ++y)
	{
		for (UINT ix = x; x < width; ++x)
		{
			if (ix < size.x && iy < size.y)
				setPixel(ix, iy, r, g, b, a);
		}
	}
}

void Bitmap::getPixel(UINT x, UINT y, XMUINT4* color) const
{
	BF(x >= 0 && y >= 0);
	BF(x < size.x && y < size.y);

	getPixel(y * size.x + x, color);
}

void Bitmap::getPixel(UINT index, XMUINT4* color) const
{
	BF(index >= 0 && index < (size.x * size.y * 4));

	color->x = pixels[index * 4];
	color->y = pixels[index * 4 + 1];
	color->z = pixels[index * 4 + 2];
	color->w = pixels[index * 4 + 3];
}

UINT Bitmap::getWidth() const
{
	return size.x;
}

UINT Bitmap::getHeight() const
{
	return size.y;
}

void Bitmap::updateTexture() const
{
	if (texture.getPtr() != nullptr)
	{
		D3D11_MAPPED_SUBRESOURCE mappedData;
		HR(Pipeline::DeviceContext->Map(texture.getPtr(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData));

		BYTE* data = static_cast<BYTE*>(mappedData.pData);
		BYTE* buffer = pixels;

		for (UINT i = 0; i < size.y; ++i)
		{
			memcpy(data, buffer, size.x * 4);
			data += mappedData.RowPitch;
			buffer += size.x * 4;
		}

		Pipeline::DeviceContext->Unmap(texture.getPtr(), 0);
	}
	else
	{
		texture.setWidth(size.x);
		texture.setHeight(size.y);
		texture.setUsage(D3D11_USAGE_DYNAMIC);
		texture.setArraySize(1);
		texture.setFormat(DXGI_FORMAT_R8G8B8A8_UNORM);
		texture.setSampleQuality(0);
		texture.setSampleCount(1);
		texture.setMiscFlags(0);
		texture.setMipLevels(1);
		texture.setCPUAccessFlags(D3D11_CPU_ACCESS_WRITE);
		texture.setBindFlags(D3D11_BIND_SHADER_RESOURCE);
		texture.setMem(pixels);

		texture.create();

		srv.create(texture.getPtr());
	}
}