#include "stdafx.h"
#include "Pipeline.h"

void Pipeline::bindRenderTargetView(const RenderTargetView& renderTargetView, const DepthStencilView& depthStencilView)
{
	DeviceContext->OMSetRenderTargets(1, renderTargetView.GetAddressOf(), depthStencilView.Get());
}

void Pipeline::bindViewport(const Viewport& viewport)
{
	DeviceContext->RSSetViewports(1, &viewport);
}

void Pipeline::bindPsResourceView(const ShaderResourceView& srv)
{
	DeviceContext->PSSetShaderResources(0, 1, srv.GetAddressOf());
}

void Pipeline::bindVertexBuffer(const Buffer& buffer, size_t stride)
{
	UINT offset = 0;
	DeviceContext->IASetVertexBuffers(0, 1, buffer.GetAddressOf(), &stride, &offset);
}

void Pipeline::bindIndexBuffer(const Buffer& buffer, DXGI_FORMAT format)
{
	DeviceContext->IASetIndexBuffer(buffer.Get(), format, 0);
}

void Pipeline::bindPixelShader(const PixelShader& shader)
{
	DeviceContext->PSSetShader(shader.Get(), 0, 0);
}

void Pipeline::bindVertexShader(const VertexShader& shader)
{
	DeviceContext->VSSetShader(shader.Get(), 0, 0);
}

void Pipeline::bindInputLayout(const InputLayout& layout)
{
	DeviceContext->IASetInputLayout(layout.Get());
}

void Pipeline::bindBlendState(const BlendState& state)
{
	DeviceContext->OMSetBlendState(state.Get(), nullptr, 0xffffffff);
}

void Pipeline::bindDepthStencilState(const DepthStencilState& state)
{
	DeviceContext->OMSetDepthStencilState(state.Get(), 1);
}

void Pipeline::bindRasterizerState(const RasterizerState& state)
{
	DeviceContext->RSSetState(state.Get());
}

void Pipeline::bindPsSamplerState(const SamplerState& state)
{
	DeviceContext->PSSetSamplers(0, 1, state.GetAddressOf());
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

void Pipeline::getDefaultDxgiDevice(DXGIDevice dxgiDevice)
{
	BF(Device.Get()->QueryInterface(__uuidof(IDXGIDevice), (LPVOID*)dxgiDevice.GetAddressOf()));
}

void Pipeline::getDefaultDxgiAdapter(DXGIAdapter dxgiAdapter)
{

}

void Pipeline::getDefaultDxgiFactory(DXGIFactory dxgiFactory)
{
}