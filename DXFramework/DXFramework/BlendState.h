#pragma once

#include "D3D11Ptr.h"
#include "D3D11Desc.h"

class BlendState : public D3D11Ptr<ID3D11BlendState>, public D3D11Desc<D3D11_BLEND_DESC>
{
public:
	BlendState();
	virtual ~BlendState();

	void create();

	void setAlphaToCoverageEnable(BOOL enable);
	void setIndependentBlendEnable(BOOL enable);
	void setBlendEnable(UINT8 renderTargetIndex, BOOL enable);
	void setSrcBlend(UINT8 renderTargetIndex, D3D11_BLEND blend);
	void setSrcBlendAlpha(UINT8 renderTargetIndex, D3D11_BLEND blend);
	void setDestBlend(UINT8 renderTargetIndex, D3D11_BLEND blend);
	void setDestBlendAlpha(UINT8 renderTargetIndex, D3D11_BLEND blend);
	void setBlendOp(UINT8 renderTargetIndex, D3D11_BLEND_OP op);
	void setBlendOpAlpha(UINT8 renderTargetIndex, D3D11_BLEND_OP op);
	void setWriteMask(UINT8 renderTargetIndex, UINT8 mask);

	void initFast(UINT8 renderTargetIndex, D3D11_BLEND srcBlend, D3D11_BLEND destBlend, D3D11_BLEND_OP op);

	void initOpaque(UINT8 renderTargetIndex);
	void initAlphaBlend(UINT8 renderTargetIndex);
	void initAdditive(UINT8 renderTargetIndex);
	void initPremultiplied(UINT8 renderTargetIndex);
};

