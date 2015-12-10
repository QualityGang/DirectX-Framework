#pragma once

#include "D3D11Ptr.h"
#include "D3D11Desc.h"

class RasterizerState : public D3D11Ptr<ID3D11RasterizerState>, public D3D11Desc<D3D11_RASTERIZER_DESC>
{
public:
	RasterizerState();
	virtual ~RasterizerState();

	void create();

	void setFillMode(D3D11_FILL_MODE mode);
	void setCullMode(D3D11_CULL_MODE mode);
	void setFrontCounterClockwise(BOOL clockwise);
	void setDepthBias(INT bias);
	void setDepthBiasClamp(FLOAT clamp);
	void setSlopeScaledDepthBias(FLOAT bias);
	void setDepthClipEnable(BOOL enable);
	void setScissorEnable(BOOL enable);
	void setMultisampleEnable(BOOL enable);
	void setAntialiasedLineEnable(BOOL enable);

	void initFast(D3D11_CULL_MODE cullMode, D3D11_FILL_MODE fillMode);

	void initCullNone();
	void initCullCockwise();
	void initCullCounterClockwise();
	void initWireframe();
};

