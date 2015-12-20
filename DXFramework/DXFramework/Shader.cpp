#include "stdafx.h"
#include "Shader.h"


Shader::Shader()
{
}

Shader::~Shader()
{
}

void Shader::setBytecode(const BYTE *bytecode, size_t size)
{
	this->bytecode = bytecode;
	this->bytecodeSize = size;
}

const BYTE* Shader::getBytecode() const
{
	return bytecode;
}

size_t Shader::getBytecodeSize() const
{
	return bytecodeSize;
}