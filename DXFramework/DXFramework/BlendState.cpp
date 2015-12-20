#include "stdafx.h"
#include "BlendState.h"

#include "Pipeline.h"


BlendState::BlendState()
{
}

BlendState::~BlendState()
{
}

void BlendState::create()
{
	HR(Pipeline::Device->CreateBlendState(&desc, ptr.GetAddressOf()));
}

void BlendState::setAlphaToCoverageEnable(BOOL enable)
{
	desc.AlphaToCoverageEnable = enable;
}

void BlendState::setIndependentBlendEnable(BOOL enable)
{
	desc.IndependentBlendEnable = enable;
}

void BlendState::setBlendEnable(UINT8 renderTargetIndex, BOOL enable)
{
	desc.RenderTarget[renderTargetIndex].BlendEnable = enable;
}

void BlendState::setSrcBlend(UINT8 renderTargetIndex, D3D11_BLEND blend)
{
	desc.RenderTarget[renderTargetIndex].SrcBlend = blend;
}

void BlendState::setSrcBlendAlpha(UINT8 renderTargetIndex, D3D11_BLEND blend)
{
	desc.RenderTarget[renderTargetIndex].SrcBlendAlpha = blend;
}

void BlendState::setDestBlend(UINT8 renderTargetIndex, D3D11_BLEND blend)
{
	desc.RenderTarget[renderTargetIndex].DestBlend = blend;
}

void BlendState::setDestBlendAlpha(UINT8 renderTargetIndex, D3D11_BLEND blend)
{
	desc.RenderTarget[renderTargetIndex].DestBlendAlpha = blend;
}

void BlendState::setBlendOp(UINT8 renderTargetIndex, D3D11_BLEND_OP op)
{
	desc.RenderTarget[renderTargetIndex].BlendOp = op;
}

void BlendState::setBlendOpAlpha(UINT8 renderTargetIndex, D3D11_BLEND_OP op)
{
	desc.RenderTarget[renderTargetIndex].BlendOpAlpha = op;
}

void BlendState::setWriteMask(UINT8 renderTargetIndex, UINT8 mask)
{
	desc.RenderTarget[renderTargetIndex].RenderTargetWriteMask = mask;
}

void BlendState::initFast(UINT8 renderTargetIndex, D3D11_BLEND srcBlend, D3D11_BLEND destBlend, D3D11_BLEND_OP op)
{
	desc.RenderTarget[renderTargetIndex].BlendEnable = (srcBlend != D3D11_BLEND_ONE) || (destBlend != D3D11_BLEND_ZERO);
	desc.RenderTarget[renderTargetIndex].SrcBlend = srcBlend;
	desc.RenderTarget[renderTargetIndex].SrcBlendAlpha = srcBlend;
	desc.RenderTarget[renderTargetIndex].DestBlend = destBlend;
	desc.RenderTarget[renderTargetIndex].DestBlendAlpha = destBlend;
	desc.RenderTarget[renderTargetIndex].BlendOp = op;
	desc.RenderTarget[renderTargetIndex].BlendOpAlpha = op;
	desc.RenderTarget[renderTargetIndex].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
}

void BlendState::initOpaque(UINT8 renderTargetIndex)
{
	initFast(renderTargetIndex, D3D11_BLEND_ONE, D3D11_BLEND_ZERO, D3D11_BLEND_OP_ADD);
}

void BlendState::initAlphaBlend(UINT8 renderTargetIndex)
{
	initFast(renderTargetIndex, D3D11_BLEND_SRC_ALPHA, D3D11_BLEND_INV_SRC_ALPHA, D3D11_BLEND_OP_ADD);
}

void BlendState::initAdditive(UINT8 renderTargetIndex)
{
	initFast(renderTargetIndex, D3D11_BLEND_SRC_ALPHA, D3D11_BLEND_ONE, D3D11_BLEND_OP_ADD);
}

void BlendState::initPremultiplied(UINT8 renderTargetIndex)
{
	initFast(renderTargetIndex, D3D11_BLEND_ONE, D3D11_BLEND_INV_SRC_ALPHA, D3D11_BLEND_OP_ADD);
}
