#pragma once

#include "RenderTargetView.h"
#include "DepthStencilView.h"
#include "ShaderResourceView.h"
#include "Buffer.h"
#include "PixelShader.h"
#include "VertexShader.h"
#include "InputLayout.h"
#include "BlendState.h"
#include "DepthStencilState.h"
#include "RasterizerState.h"
#include "SamplerState.h"

class Pipeline
{
public:
	Pipeline() = delete;

	static void bindRenderTargetView(const RenderTargetView &renderTargetView, const DepthStencilView &depthStencilView);
	static void bindViewport(const D3D11_VIEWPORT &viewport);
	static void bindPsResourceView(const ShaderResourceView &srv);
	static void bindVertexBuffer(const Buffer &buffer, size_t stride);
	static void bindIndexBuffer(const Buffer &buffer, DXGI_FORMAT format);
	static void bindPixelShader(const PixelShader &shader);
	static void bindVertexShader(const VertexShader &shader);
	static void bindInputLayout(const InputLayout &layout);
	static void bindBlendState(const BlendState &state);
	static void bindDepthStencilState(const DepthStencilState &state);
	static void bindRasterizerState(const RasterizerState &state);
	static void bindPsSamplerState(const SamplerState &state);

	static void setPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY topology);
	static void draw(UINT vertexCount, UINT startVertexLocation);
	static void drawIndexed(UINT indexCount, UINT startIndexLocation, UINT baseVertexLocation);

	static void getDefaultDxgiDevice(IDXGIDevice **dxgiDevice);
	static void getDefaultDxgiAdapter(IDXGIAdapter **dxgiAdapter, IDXGIDevice *dxgiDevice);
	static void getDefaultDxgiFactory(IDXGIFactory **dxgiFactory, IDXGIAdapter *dxgiAdapter);

	static ComPtr<ID3D11Device> Device;
	static ComPtr<ID3D11DeviceContext> DeviceContext;
private:
	class Initializer
	{
	public:
		Initializer();
	};

	static Initializer ___initializer;
};

