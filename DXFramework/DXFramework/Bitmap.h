#pragma once

#include "Texture.h"
#include "Texture2D.h"

class Bitmap
{
public:
	Bitmap(UINT width, UINT height, const BYTE* pixels);
	Bitmap(UINT width, UINT height, BYTE r, BYTE g, BYTE b, BYTE a);
	Bitmap(const Bitmap& other);
	
	Bitmap operator=(const Bitmap& other);

	virtual ~Bitmap();


	void SetPixel(UINT x, UINT y, BYTE r, BYTE g, BYTE b, BYTE a);
	void SetPixel(UINT index, BYTE r, BYTE g, BYTE b, BYTE a);
	void SetPixelRect(UINT x, UINT y, UINT width, UINT height, BYTE r, BYTE g, BYTE b, BYTE a);

	void GetPixel(UINT x, UINT y, XMUINT4* color) const;
	void GetPixel(UINT index, XMUINT4* color) const;

	UINT GetWidth() const;
	UINT GetHeight() const;
private:
	void UpdateTexture() const;

	XMUINT2 size;
	BYTE* pixels;

	mutable Texture2D texture;
	mutable ShaderResourceView srv;
};

