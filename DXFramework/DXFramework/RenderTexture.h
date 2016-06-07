#pragma once

#include "RenderTarget.h"
#include "Bitmap.h"

class RenderTexture : public RenderTarget
{
public:
	RenderTexture(UINT width, UINT height);
	RenderTexture(const Bitmap &bitmap);
	virtual ~RenderTexture();

	void getShaderResourceView(ID3D11ShaderResourceView **srv) const { *srv = shaderResourceView; }
private:
	ID3D11ShaderResourceView *shaderResourceView;
};

