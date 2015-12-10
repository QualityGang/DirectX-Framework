#pragma once

#include "D3D11Ptr.h"

class DepthStencilView : public D3D11Ptr<ID3D11DepthStencilView>
{
public:
	DepthStencilView();
	virtual ~DepthStencilView();

	void create(ID3D11Resource *resource);

	void clear(FLOAT depth, UINT8 stencil);
};

