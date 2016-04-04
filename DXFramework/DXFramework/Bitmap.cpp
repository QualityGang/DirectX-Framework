#include "stdafx.h"
#include "Bitmap.h"

#include "D3D11Renderer.h"

Bitmap::Bitmap() : width(0), height(0)
{
}

Bitmap::Bitmap(UINT width, UINT height, const BYTE *pixels) : 
	width(width), height(height)
{
	UINT size = width * height * 4;
	buffer = new BYTE[size];

	if (pixels)
		memcpy(buffer, pixels, size);
	else
		ZeroMemory(buffer, size);
}

Bitmap::~Bitmap()
{
	delete[] buffer;
	SafeRelease(texture);
	SafeRelease(shaderResourceView);
}

Bitmap::Bitmap(const Bitmap &other)
{
	width = other.width;
	height = other.height;
	texture = nullptr;
	shaderResourceView = nullptr;

	UINT size = width * height * 4;
	buffer = new BYTE[size];
	memcpy(buffer, other.buffer, size);
}

Bitmap::Bitmap(Bitmap &&other)
{
	buffer = other.buffer;
	width = other.width;
	height = other.height;
	texture = other.texture;
	shaderResourceView = other.shaderResourceView;
	
	other.buffer = nullptr;
	other.width = 0;
	other.height = 0;
	other.texture = nullptr;
	other.shaderResourceView = nullptr;
}

Bitmap& Bitmap::operator=(const Bitmap &other)
{
	if (this != &other)
		*this = std::move(Bitmap(other));

	return *this;
}

Bitmap& Bitmap::operator=(Bitmap &&other)
{
	if (this != &other)
	{
		delete[] buffer;
		SafeRelease(texture);
		SafeRelease(shaderResourceView);

		buffer = other.buffer;
		width = other.width;
		height = other.height;
		dirty = other.dirty;
		texture = other.texture;
		shaderResourceView = other.shaderResourceView;

		other.buffer = nullptr;
		other.width = 0;
		other.height = 0;
		other.texture = nullptr;
		other.shaderResourceView = nullptr;
	}

	return *this;
}

void Bitmap::setPixel(UINT index, BYTE r, BYTE g, BYTE b, BYTE a)
{
	BYTE *pixel = &buffer[index * 4];
	*pixel++ = r;
	*pixel++ = g;
	*pixel++ = b;
	*pixel   = a;

	dirty = true;
}

void Bitmap::setPixel(UINT x, UINT y, BYTE r, BYTE g, BYTE b, BYTE a)
{
	setPixel(y * width + x, r, g, b, a);
}

void Bitmap::getPixel(UINT index, XMUINT4 *color) const
{
	BYTE *pixel = &buffer[index * 4];
	color->x = *pixel++;
	color->y = *pixel++;
	color->z = *pixel++;
	color->w = *pixel  ;
}

void Bitmap::getPixel(UINT x, UINT y, XMUINT4 *color) const
{
	getPixel(y * width + x, color);
}

UINT Bitmap::getWidth() const
{
	return width;
}

UINT Bitmap::getHeight() const
{
	return height;
}

void Bitmap::getTexture2D(ID3D11Texture2D **tex) const
{
	if (dirty)
		if (updateTexture())
			dirty = false;
		else
		{
			*tex = nullptr;
			return;
		}

	*tex = texture;
}

void Bitmap::getShaderResourceView(ID3D11ShaderResourceView **srv) const
{
	if (dirty)
		if (updateTexture())
			dirty = false;
		else
		{
			*srv = nullptr;
			return;
		}

	*srv = shaderResourceView;
}

bool Bitmap::updateTexture() const
{
	if (!buffer)
		return false;

	if (texture)
	{
		BYTE *texBuff;
		UINT rowWidth;
		D3D11Renderer::Map(texture, D3D11_MAP_WRITE_DISCARD, (void**)&texBuff, &rowWidth);

		if (!texBuff)
			return false;

		BYTE *buffer = this->buffer;

		for (UINT i = 0; i < height; i++)
		{
			memcpy(texBuff, buffer, width * 4);
			texBuff += rowWidth;
			buffer += width * 4;
		}

		D3D11Renderer::Unmap(texture);

		return true;
	}
	else
	{
		D3D11Renderer::CreateTexture2D(width, height,
			DXGI_FORMAT_R8G8B8A8_UNORM, D3D11_BIND_SHADER_RESOURCE,
			D3D11_USAGE_DYNAMIC, 1, 0, buffer, width * 4, &texture);

		if (!texture)
			return false;
		
		D3D11Renderer::CreateShaderResourceView(texture, &shaderResourceView);
		
		return shaderResourceView != nullptr;
	}
}
