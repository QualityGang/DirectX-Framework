#include "stdafx.h"
#include "Buffer.h"



Buffer::Buffer()
{
}

Buffer::~Buffer()
{
}

void Buffer::create()
{
	
}

void Buffer::setByteWidth(UINT width)
{
	desc.ByteWidth = width;
}

void Buffer::setUsage(D3D11_USAGE usage)
{
	desc.Usage = usage;
}

void Buffer::setBindFlags(UINT flags)
{
	desc.BindFlags = flags;
}

void Buffer::setCPUAccessFlags(UINT flags)
{
	desc.CPUAccessFlags = flags;
}

void Buffer::setMiscFlags(UINT flags)
{
	desc.MiscFlags = flags;
}

void Buffer::StructureByteStride(UINT stride)
{
	desc.StructureByteStride = stride;
}
