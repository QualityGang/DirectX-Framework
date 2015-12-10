#include "stdafx.h"
#include "Texture2D.h"



Texture2D::Texture2D()
{
}

Texture2D::~Texture2D()
{
}

void Texture2D::create()
{

}

void Texture2D::setArraySize(UINT size)
{
	desc.ArraySize = size;
}

void Texture2D::setMipLevels(UINT levels)
{
	desc.MipLevels = levels;
}

void Texture2D::setBindFlags(UINT flags)
{
	desc.BindFlags = flags;
}

void Texture2D::setFormat(DXGI_FORMAT format)
{
	desc.Format = format;
}

void Texture2D::setWidth(UINT width)
{
	desc.Width = width;
}

void Texture2D::setHeight(UINT height)
{
	desc.Height = height;
}

void Texture2D::setSampleCount(UINT count)
{
	desc.SampleDesc.Count = count;
}

void Texture2D::setSampleQuality(UINT quality)
{
	desc.SampleDesc.Quality = quality;
}

void Texture2D::setUsage(D3D11_USAGE usage)
{
	desc.Usage = usage;
}

void Texture2D::setCPUAccessFlags(UINT flags)
{
	desc.CPUAccessFlags = flags;
}

void Texture2D::setMiscFlags(UINT flags)
{
	desc.MiscFlags = flags;
}
