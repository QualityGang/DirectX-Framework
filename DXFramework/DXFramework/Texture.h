#pragma once

#include "ShaderResourceView.h"

class Texture
{
public:
	Texture(LPCSTR filename);
	virtual ~Texture();

	const XMUINT2& getSize() const;

	const ShaderResourceView& getSRV() const;
private:
	XMUINT2 size;
	ShaderResourceView srv;
};

