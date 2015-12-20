#include "stdafx.h"
#include "RasterizerState.h"
#include "Pipeline.h"


RasterizerState::RasterizerState()
{
}

RasterizerState::~RasterizerState()
{
}

void RasterizerState::create()
{
	BF(Pipeline::Device.Get()->CreateRasterizerState(&getDesc(), ptr.GetAddressOf()));
}

void RasterizerState::setFillMode(D3D11_FILL_MODE mode)
{
	desc.FillMode = mode;
}

void RasterizerState::setCullMode(D3D11_CULL_MODE mode)
{
	desc.CullMode = mode;
}

void RasterizerState::setFrontCounterClockwise(BOOL clockwise)
{
	desc.FrontCounterClockwise = clockwise;
}

void RasterizerState::setDepthBias(INT bias)
{
	desc.DepthBias = bias;
}

void RasterizerState::setDepthBiasClamp(FLOAT clamp)
{
	desc.DepthBiasClamp = clamp;
}

void RasterizerState::setSlopeScaledDepthBias(FLOAT bias)
{
	desc.SlopeScaledDepthBias = bias;
}

void RasterizerState::setDepthClipEnable(BOOL enable)
{
	desc.DepthClipEnable = enable;
}

void RasterizerState::setScissorEnable(BOOL enable)
{
	desc.ScissorEnable = enable;
}

void RasterizerState::setMultisampleEnable(BOOL enable)
{
	desc.MultisampleEnable = enable;
}

void RasterizerState::setAntialiasedLineEnable(BOOL enable)
{
	desc.AntialiasedLineEnable = enable;
}

void RasterizerState::initFast(D3D11_CULL_MODE cullMode, D3D11_FILL_MODE fillMode)
{
	desc.CullMode = cullMode;
	desc.FillMode = fillMode;
	desc.DepthClipEnable = TRUE;
}

void RasterizerState::initCullNone()
{
	initFast(D3D11_CULL_NONE, D3D11_FILL_SOLID);
}

void RasterizerState::initCullCockwise()
{
	initFast(D3D11_CULL_FRONT, D3D11_FILL_SOLID);
}

void RasterizerState::initCullCounterClockwise()
{
	initFast(D3D11_CULL_BACK, D3D11_FILL_SOLID);
}

void RasterizerState::initWireframe()
{
	initFast(D3D11_CULL_NONE, D3D11_FILL_WIREFRAME);
}