#include "stdafx.h"
#include "Pipeline.h"


ComPtr<ID3D11Device> Pipeline::Device;
ComPtr<ID3D11DeviceContext> Pipeline::DeviceContext;


void Pipeline::bindRenderTargetView(const RenderTargetView &renderTargetView, const DepthStencilView &depthStencilView)
{
	RenderTargetView::RawPtr ptr = renderTargetView.getPtr();
	DeviceContext->OMSetRenderTargets(1, &ptr, depthStencilView.getPtr());
}

void Pipeline::bindViewport(const D3D11_VIEWPORT &viewport)
{
	DeviceContext->RSSetViewports(1, &viewport);
}

void Pipeline::bindPsResourceView(const ShaderResourceView &srv)
{
	ShaderResourceView::RawPtr ptr = srv.getPtr();
	DeviceContext->PSSetShaderResources(0, 1, &ptr);
}

void Pipeline::bindVertexBuffer(const Buffer &buffer, size_t stride)
{
	Buffer::RawPtr ptr = buffer.getPtr();
	UINT offset = 0;
	DeviceContext->IASetVertexBuffers(0, 1, &ptr, &stride, &offset);
}

void Pipeline::bindIndexBuffer(const Buffer &buffer, DXGI_FORMAT format)
{
	DeviceContext->IASetIndexBuffer(buffer.getPtr(), format, 0);
}

void Pipeline::bindPixelShader(const PixelShader &shader)
{
	DeviceContext->PSSetShader(shader.getPtr(), nullptr, 0);
}

void Pipeline::bindVertexShader(const VertexShader &shader)
{
	DeviceContext->VSSetShader(shader.getPtr(), nullptr, 0);
}

void Pipeline::bindInputLayout(const InputLayout &layout)
{
	DeviceContext->IASetInputLayout(layout.getPtr());
}

void Pipeline::bindBlendState(const BlendState &state)
{
	DeviceContext->OMSetBlendState(state.getPtr(), nullptr, 0xffffffff);
}

void Pipeline::bindDepthStencilState(const DepthStencilState &state)
{
	DeviceContext->OMSetDepthStencilState(state.getPtr(), 1);
}

void Pipeline::bindRasterizerState(const RasterizerState &state)
{
	DeviceContext->RSSetState(state.getPtr());
}

void Pipeline::bindPsSamplerState(const SamplerState &state)
{
	SamplerState::RawPtr ptr = state.getPtr();
	DeviceContext->PSSetSamplers(0, 1, &ptr);
}

void Pipeline::setPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY topology)
{
	DeviceContext->IASetPrimitiveTopology(topology);
}

void Pipeline::draw(UINT vertexCount, UINT startVertexLocation)
{
	DeviceContext->Draw(vertexCount, startVertexLocation);
}

void Pipeline::drawIndexed(UINT indexCount, UINT startIndexLocation, UINT baseVertexLocation)
{
	DeviceContext->DrawIndexed(indexCount, startIndexLocation, baseVertexLocation);
}

void Pipeline::getDefaultDxgiDevice(IDXGIDevice **dxgiDevice)
{
	BF(Device.Get()->QueryInterface(__uuidof(IDXGIDevice), (void**)dxgiDevice));
}

void Pipeline::getDefaultDxgiAdapter(IDXGIAdapter **dxgiAdapter)
{

}

void Pipeline::getDefaultDxgiFactory(IDXGIFactory **dxgiFactory)
{
}

Pipeline::Initializer::Initializer()
{
	UINT createDeviceFlags = 0;
#if _DEBUG
	createDeviceFlags = D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_0;

	HR(D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr,
						 createDeviceFlags, &featureLevel, 1, D3D11_SDK_VERSION,
						 Device.GetAddressOf(), nullptr, DeviceContext.GetAddressOf()));
}
