#pragma once

#include "D3D11Ptr.h"
#include "D3D11Desc.h"

class RenderTargetView : public D3D11Ptr<ID3D11RenderTargetView>, public D3D11Desc<D3D11_RENDER_TARGET_VIEW_DESC>
{
public:
	RenderTargetView();
	virtual ~RenderTargetView();

	void create(ID3D11Resource *resource);

	void clear(float r, float g, float b, float a);
};

