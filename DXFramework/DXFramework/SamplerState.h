#pragma once

#include "D3D11Ptr.h"
#include "D3D11Desc.h"

class SamplerState : public D3D11Ptr<ID3D11SamplerState>, public D3D11Desc<D3D11_SAMPLER_DESC>
{
public:
	SamplerState();
	virtual ~SamplerState();

	void create();

	void setFilter(D3D11_FILTER filter);
	void setAddressU(D3D11_TEXTURE_ADDRESS_MODE mode);
	void setAddressV(D3D11_TEXTURE_ADDRESS_MODE mode);
	void setAddressW(D3D11_TEXTURE_ADDRESS_MODE mode);
	void setMipLODBias(FLOAT bias);
	void setMaxAnisotropy(UINT maxAnisotropy);
	void setComparisonFunc(D3D11_COMPARISON_FUNC func);
	void setBorderColor(FLOAT r, FLOAT g, FLOAT b, FLOAT a);
	void setMinLOD(FLOAT minLOD);
	void setMaxLOD(FLOAT maxLOD);

	void initFast(D3D11_FILTER filter, D3D11_TEXTURE_ADDRESS_MODE addressMode, UINT maxAnisotropy);

	void initPointWrap();
	void initPointClamp();
	void initLinearWrap();
	void initLinearClamp();
	void initAnisotropicWrap(UINT maxAnisotropy);
	void initAnisotropicClamp(UINT maxAnisotropy);
};

