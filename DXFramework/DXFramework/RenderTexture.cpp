#include "stdafx.h"
#include "RenderTexture.h"

#include "D3D11Renderer.h"
#include "Settings.h"


RenderTexture::RenderTexture(UINT width, UINT height)
{
	ID3D11Texture2D *texture;

	D3D11Renderer::CreateTexture2D(width, height,
		DXGI_FORMAT_R8G8B8A8_UNORM, D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET,
		D3D11_USAGE_DEFAULT, SAMPLE_COUNT, SAMPLE_QUALITY, nullptr, 0, &texture);

	D3D11Renderer::CreateShaderResourceView(texture, &shaderResourceView);

	D3D11Renderer::CreateRenderTargetView(texture, &renderTargetView);
	D3D11Renderer::CreateDepthStencilView(width, height, 1, 0, &depthStencilView);

	updateSize((float)width, (float)height);

	SafeRelease(texture);
}

RenderTexture::RenderTexture(const Bitmap &bitmap)
{
	ID3D11Texture2D *buffer;
	bitmap.getTexture2D(&buffer);

	D3D11_TEXTURE2D_DESC bufferDesc;
	buffer->GetDesc(&bufferDesc);
	bufferDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	bufferDesc.SampleDesc.Count = 1;
	bufferDesc.SampleDesc.Quality = 0;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.CPUAccessFlags = 0;

	ID3D11Texture2D *texture;
	HRESULT hr = D3D11Renderer::Device->CreateTexture2D(&bufferDesc, nullptr, &texture);

	if (FAILED(hr))
	{
		PrintError(hr);
		shaderResourceView = nullptr;
		return;
	}

	D3D11Renderer::CreateShaderResourceView(texture, &shaderResourceView);

	D3D11Renderer::CreateRenderTargetView(texture, &renderTargetView);
	D3D11Renderer::CreateDepthStencilView(bufferDesc.Width, bufferDesc.Height, 1, 0, &depthStencilView);

	updateSize((float)bufferDesc.Width, (float)bufferDesc.Height);

	SafeRelease(texture);
}

RenderTexture::~RenderTexture()
{
	SafeRelease(shaderResourceView);
	SafeRelease(renderTargetView);
	SafeRelease(depthStencilView);
}
