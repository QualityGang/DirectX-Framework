#pragma once

#include "ShaderResourceView.h"

class FIHelper
{
public:
	FIHelper();
	virtual ~FIHelper();

	static void createSRV(LPCSTR filename, ShaderResourceView *srv, UINT *width, UINT *height);
private:
	class Initializer
	{
	public:
		Initializer();
		virtual ~Initializer();
	};

	static Initializer ___initializer;
};

