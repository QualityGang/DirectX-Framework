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


	void setPixel(UINT x, UINT y, BYTE r, BYTE g, BYTE b, BYTE a);
	void setPixel(UINT index, BYTE r, BYTE g, BYTE b, BYTE a);
	void setPixelRect(UINT x, UINT y, UINT width, UINT height, BYTE r, BYTE g, BYTE b, BYTE a);

	void getPixel(UINT x, UINT y, XMUINT4* color) const;
	void getPixel(UINT index, XMUINT4* color) const;

	UINT getWidth() const;
	UINT getHeight() const;
private:
	void updateTexture() const;

	XMUINT2 size;
	BYTE* pixels;

	mutable Texture2D texture;
	mutable ShaderResourceView srv;
};

