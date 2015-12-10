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
	m_Bytecode = bytecode;
	m_BytecodeSize = size;
}

const BYTE* Shader::getBytecode() const
{
	return m_Bytecode;
}

size_t Shader::getBytecodeSize() const
{
	return m_BytecodeSize;
}