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
	BF(Pipeline::Device.Get()->CreateRenderTargetView(resource, &getDesc(), ptr.GetAddressOf()));
}

void RenderTargetView::clear(float r, float g, float b, float a)
{
	float color[] = { r, g, b, a };

}