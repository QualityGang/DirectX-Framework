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
	HR(Pipeline::Device.Get()->CreateVertexShader(bytecode, bytecodeSize, nullptr, ptr.GetAddressOf()));
}