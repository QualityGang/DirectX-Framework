#include "stdafx.h"
#include "RenderTarget.h"

#include "D3D11Renderer.h"


RenderTarget::RenderTarget()
{
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
}

RenderTarget::~RenderTarget()
{
}

void RenderTarget::clear(float r, float g, float b, float a)
{
	FLOAT color[4] = { r, g, b, a };
	D3D11Renderer::DeviceContext->ClearRenderTargetView(renderTargetView, color);
	D3D11Renderer::DeviceContext->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

void RenderTarget::updateSize(float width, float height)
{
	if (width == 0.0f || height == 0.0f)
		projectionMatrix = XMMatrixIdentity();
	else
		projectionMatrix = XMMatrixOrthographicOffCenterLH(0.0f, width, height, 0.0f, 0.0f, 1.0f);

	viewport.Width = width;
	viewport.Height = height;
}

ID3D11RenderTargetView* RenderTarget::getRenderTargetView() const
{
	return renderTargetView;
}

ID3D11DepthStencilView* RenderTarget::getDepthStencilView() const
{
	return depthStencilView;
}

const XMMATRIX& RenderTarget::getProjectionMatrix() const
{
	return projectionMatrix;
}

const D3D11_VIEWPORT& RenderTarget::getViewport() const
{
	return viewport;
}
