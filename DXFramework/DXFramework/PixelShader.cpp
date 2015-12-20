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
	HR(Pipeline::Device->CreatePixelShader(bytecode, bytecodeSize, nullptr, ptr.GetAddressOf()));
}