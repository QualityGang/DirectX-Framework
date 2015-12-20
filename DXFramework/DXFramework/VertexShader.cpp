#include "stdafx.h"
#include "VertexShader.h"
#include "Pipeline.h"


VertexShader::VertexShader()
{
}

VertexShader::~VertexShader()
{
}

void VertexShader::create()
{
	BF(Pipeline::Device.Get()->CreateVertexShader(getBytecode(), getBytecodeSize(), nullptr, ptr.GetAddressOf()));
}