#include "stdafx.h"
#include "PixelShader.h"
#include "Pipeline.h"


PixelShader::PixelShader()
{
}

PixelShader::~PixelShader()
{
}

void PixelShader::create()
{
	BF(Pipeline::Device.Get()->CreatePixelShader(getBytecode(), getBytecodeSize(), nullptr, ptr.GetAddressOf()));
}