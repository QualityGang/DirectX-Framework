#include "stdafx.h"
#include "DepthStencilState.h"
#include "Pipeline.h"


DepthStencilState::DepthStencilState()
{
}

DepthStencilState::~DepthStencilState()
{
}

void DepthStencilState::create()
{
	HR(Pipeline::Device->CreateDepthStencilState(&getDesc(), ptr.GetAddressOf()));
}

void DepthStencilState::setDepthEnable(BOOL enable)
{
	desc.DepthEnable = enable;
}

void DepthStencilState::setDepthWriteMask(D3D11_DEPTH_WRITE_MASK mask)
{
	desc.DepthWriteMask = mask;
}

void DepthStencilState::setDepthComparisonFunc(D3D11_COMPARISON_FUNC func)
{
	desc.DepthFunc = func;
}

void DepthStencilState::setStencilEnable(BOOL enable)
{
	desc.StencilEnable = enable;
}

void DepthStencilState::setStencilReadMask(UINT8 mask)
{
	desc.StencilReadMask = mask;
}

void DepthStencilState::setStencilWriteMask(UINT8 mask)
{
	desc.StencilWriteMask = mask;
}

void DepthStencilState::setFrontFaceStencilFailOp(D3D11_STENCIL_OP op)
{
	desc.FrontFace.StencilFailOp = op;
}

void DepthStencilState::setFrontFaceStencilDepthFailOp(D3D11_STENCIL_OP op)
{
	desc.FrontFace.StencilDepthFailOp = op;
}

void DepthStencilState::setFrontFaceStencilPassOp(D3D11_STENCIL_OP op)
{
	desc.FrontFace.StencilPassOp = op;
}

void DepthStencilState::setFrontFaceStencilComparisonFunc(D3D11_COMPARISON_FUNC func)
{
	desc.FrontFace.StencilFunc = func;
}

void DepthStencilState::setBackFaceStencilFailOp(D3D11_STENCIL_OP op)
{
	desc.BackFace.StencilFailOp = op;
}

void DepthStencilState::setBackFaceStencilDepthFailOp(D3D11_STENCIL_OP op)
{
	desc.BackFace.StencilDepthFailOp = op;
}

void DepthStencilState::setBackFaceStencilPassOp(D3D11_STENCIL_OP op)
{
	desc.BackFace.StencilPassOp = op;
}

void DepthStencilState::setBackFaceStencilComparisonFunc(D3D11_COMPARISON_FUNC func)
{
	desc.BackFace.StencilFunc = func;
}

void DepthStencilState::initFast(bool enable, bool writable)
{
	desc.DepthEnable = enable;
	desc.DepthWriteMask = writable ? D3D11_DEPTH_WRITE_MASK_ALL : D3D11_DEPTH_WRITE_MASK_ZERO;
	desc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
	desc.StencilEnable = false;
	//desc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
	//desc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;
	//desc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	//desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	//desc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	//desc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	//desc.BackFace = depthStencilStateDesc.FrontFace;
}

void DepthStencilState::initDepthNone()
{
	initFast(false, false);
}

void DepthStencilState::initDepthDefault()
{
	initFast(true, true);
}

void DepthStencilState::initDepthRead()
{
	initFast(true, false);
}