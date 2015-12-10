#pragma once

#include "D3D11Ptr.h"
#include "D3D11Desc.h"
#include "D3D11SubResData.h"

class Texture2D : public D3D11Ptr<ID3D11Texture2D>, public D3D11Desc<D3D11_TEXTURE2D_DESC>
{
public:
	Texture2D();
	virtual ~Texture2D();

	void create();

	void setArraySize(UINT size);
	void setMipLevels(UINT levels);
	void setBindFlags(UINT flags);
	void setFormat(DXGI_FORMAT format);
	void setWidth(UINT width);
	void setHeight(UINT height);
	void setSampleCount(UINT count);
	void setSampleQuality(UINT quality);
	void setUsage(D3D11_USAGE usage);
	void setCPUAccessFlags(UINT flags);
	void setMiscFlags(UINT flags);
};

