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
	BF(Pipeline::Device.Get()->CreateDepthStencilView(resource, nullptr, ptr.GetAddressOf()));
}

void DepthStencilView::clear(FLOAT depth, UINT8 stencil)
{
	
}