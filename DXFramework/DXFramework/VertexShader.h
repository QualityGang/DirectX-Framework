#pragma once

#include "D3D11Ptr.h"
#include "Shader.h"

class VertexShader : public Shader, public D3D11Ptr<ID3D11VertexShader>
{
public:
	VertexShader();
	virtual ~VertexShader();

	void create();
};

