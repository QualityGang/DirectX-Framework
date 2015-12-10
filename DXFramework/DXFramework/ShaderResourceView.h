#pragma once

#include "D3D11Ptr.h"

class ShaderResourceView : public D3D11Ptr<ID3D11ShaderResourceView>
{
public:
	ShaderResourceView();
	virtual ~ShaderResourceView();

	void create(ID3D11Resource *resource);
};

