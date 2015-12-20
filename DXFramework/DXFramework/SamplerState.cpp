#include "stdafx.h"
#include "SamplerState.h"
#include "Pipeline.h"


SamplerState::SamplerState()
{
}

SamplerState::~SamplerState()
{
}

void SamplerState::create()
{
	BF(Pipeline::Device.Get()->CreateSamplerState(&getDesc(), ptr.GetAddressOf()));
}

void SamplerState::setFilter(D3D11_FILTER filter)
{
	desc.Filter = filter;
}

void SamplerState::setAddressU(D3D11_TEXTURE_ADDRESS_MODE mode)
{
	desc.AddressU = mode;
}

void SamplerState::setAddressV(D3D11_TEXTURE_ADDRESS_MODE mode)
{
	desc.AddressU = mode;
}

void SamplerState::setAddressW(D3D11_TEXTURE_ADDRESS_MODE mode)
{
	desc.AddressW = mode;
}

void SamplerState::setMipLODBias(FLOAT bias)
{
	desc.MipLODBias = bias;
}

void SamplerState::setMaxAnisotropy(UINT maxAnisotropy)
{
	desc.MaxAnisotropy = maxAnisotropy;
}

void SamplerState::setComparisonFunc(D3D11_COMPARISON_FUNC func)
{
	desc.ComparisonFunc = func;
}

void SamplerState::setBorderColor(FLOAT r, FLOAT g, FLOAT b, FLOAT a)
{
	desc.BorderColor[0] = r;
	desc.BorderColor[1] = g;
	desc.BorderColor[2] = b;
	desc.BorderColor[3] = a;
}

void SamplerState::setMinLOD(FLOAT minLOD)
{
	desc.MinLOD = minLOD;
}

void SamplerState::setMaxLOD(FLOAT maxLOD)
{
	desc.MaxLOD = maxLOD;
}

void SamplerState::initFast(D3D11_FILTER filter, D3D11_TEXTURE_ADDRESS_MODE addressMode, UINT maxAnisotropy)
{
	desc.Filter = filter;
	desc.AddressU = addressMode;
	desc.AddressV = addressMode;
	desc.AddressW = addressMode;
	desc.MaxAnisotropy = maxAnisotropy;
	desc.MaxLOD = D3D11_FLOAT32_MAX;
	desc.ComparisonFunc = D3D11_COMPARISON_NEVER;
}

void SamplerState::initPointWrap()
{
	initFast(D3D11_FILTER_MIN_MAG_MIP_POINT, D3D11_TEXTURE_ADDRESS_WRAP, 0);
}

void SamplerState::initPointClamp()
{
	initFast(D3D11_FILTER_MIN_MAG_MIP_POINT, D3D11_TEXTURE_ADDRESS_CLAMP, 0);
}

void SamplerState::initLinearWrap()
{
	initFast(D3D11_FILTER_MIN_MAG_MIP_LINEAR, D3D11_TEXTURE_ADDRESS_WRAP, 0);
}

void SamplerState::initLinearClamp()
{
	initFast(D3D11_FILTER_MIN_MAG_MIP_LINEAR, D3D11_TEXTURE_ADDRESS_CLAMP, 0);
}

void SamplerState::initAnisotropicWrap(UINT maxAnisotropy)
{
	initFast(D3D11_FILTER_ANISOTROPIC, D3D11_TEXTURE_ADDRESS_WRAP, maxAnisotropy);
}

void SamplerState::initAnisotropicClamp(UINT maxAnisotropy)
{
	initFast(D3D11_FILTER_ANISOTROPIC, D3D11_TEXTURE_ADDRESS_CLAMP, maxAnisotropy);
}