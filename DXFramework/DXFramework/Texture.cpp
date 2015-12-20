#include "stdafx.h"
#include "Texture.h"

#include "FIHelper.h"


Texture::Texture(LPCSTR filename)
{
	FIHelper::createSRV(filename, &srv, &size.x, &size.y);
}

Texture::~Texture()
{
}

const XMUINT2& Texture::getSize() const
{
	return size;
}

const ShaderResourceView& Texture::getSRV() const
{
	return srv;
}