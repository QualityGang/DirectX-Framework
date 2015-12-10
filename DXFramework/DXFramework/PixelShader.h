#pragma once

#include "D3D11Ptr.h"
#include "Shader.h"

class PixelShader : public Shader, public D3D11Ptr<ID3D11PixelShader>
{
public:
	PixelShader();
	virtual ~PixelShader();

	void create();
};

