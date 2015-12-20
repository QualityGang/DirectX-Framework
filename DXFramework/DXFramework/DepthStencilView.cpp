#include "stdafx.h"
#include "DepthStencilView.h"
#include "Pipeline.h"


DepthStencilView::DepthStencilView()
{
}

DepthStencilView::~DepthStencilView()
{
}

void DepthStencilView::create(ID3D11Resource *resource)
{
	HR(Pipeline::Device->CreateDepthStencilView(resource, nullptr, ptr.GetAddressOf()));
}

void DepthStencilView::clear(FLOAT depth, UINT8 stencil)
{
	Pipeline::DeviceContext->ClearDepthStencilView(ptr.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, depth, stencil);
}