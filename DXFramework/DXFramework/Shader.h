#pragma once

#include "NonCopyable.h"

class Shader : private NonCopyable 
{
public:
	Shader();
	virtual ~Shader();

	void setBytecode(const BYTE *bytecode, size_t size);

	const BYTE *getBytecode() const;
	size_t getBytecodeSize() const;
protected:
	const BYTE *bytecode;
	size_t bytecodeSize;
};

