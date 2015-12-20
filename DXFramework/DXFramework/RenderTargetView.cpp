#include "stdafx.h"
#include "RenderTargetView.h"
#include "Pipeline.h"


RenderTargetView::RenderTargetView()
{
}

RenderTargetView::~RenderTargetView()
{
}

void RenderTargetView::create(ID3D11Resource *resource)
{
	HR(Pipeline::Device->CreateRenderTargetView(resource, nullptr, ptr.GetAddressOf()));
}

void RenderTargetView::clear(float r, float g, float b, float a)
{
	FLOAT color[4] = { r, g, b, a };
	Pipeline::DeviceContext->ClearRenderTargetView(ptr.Get(), color);
}