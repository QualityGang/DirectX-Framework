#pragma once

typedef ComPtr<IDXGISwapChain> SwapChain;
typedef ComPtr<ID3D11Device> Device;
typedef ComPtr<ID3D11DeviceContext> DeviceContext;
typedef ComPtr<ID3D11Texture2D> Texture2D;
typedef ComPtr<ID3D11RenderTargetView> RenderTargetView;
typedef ComPtr<ID3D11DepthStencilView> DepthStencilView;
typedef ComPtr<IDXGIDevice> DXGIDevice;
typedef ComPtr<IDXGIAdapter> DXGIAdapter;
typedef ComPtr<IDXGIFactory> DXGIFactory;
typedef ComPtr<IDXGIOutput> DXGIOutput;
typedef ComPtr<ID3D11BlendState> BlendState;
typedef ComPtr<ID3D11DepthStencilState> DepthStencilState;
typedef ComPtr<ID3D11RasterizerState> RasterizerState;
typedef ComPtr<ID3D11SamplerState> SamplerState;
typedef ComPtr<ID3DBlob> Blob;
typedef ComPtr<ID3D11VertexShader> VertexShader;
typedef ComPtr<ID3D11PixelShader> PixelShader;
typedef ComPtr<ID3D11Buffer> Buffer;
typedef ComPtr<ID3D11InputLayout> InputLayout;
typedef ComPtr<ID3D11ShaderResourceView> ShaderResourceView;

typedef D3D11_VIEWPORT Viewport;


class Pipeline
{
	static void bindRenderTargetView(const RenderTargetView &renderTargetView, const DepthStencilView &depthStencilView);
	static void bindViewport(const Viewport &viewport);
	static void bindPsResourceView(const ShaderResourceView &srv);
	static void bindVertexBuffer(const Buffer &buffer, size_t stride);
	static void bindIndexBuffer(const Buffer &buffer, DXGI_FORMAT format);
	static void bindPixelShader(const PixelShader &shader);
	static void bindVertexShader(const VertexShader &shader);
	static void bindInputLayout(const InputLayout &layout);
	static void bindBlendState(const BlendState &state);
	static void bindDepthStencilState(const DepthStencilState &state);
	static void bindRasterizerState(const RasterizerState&state);
	static void bindPsSamplerState(const SamplerState &state);


	static void setPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY topology);
	static void draw(UINT vertexCount, UINT startVertexLocation);
	static void drawIndexed(UINT indexCount, UINT startIndexLocation, UINT baseVertexLocation);


	static void getDefaultDxgiDevice(DXGIDevice dxgiDevice);
	static void getDefaultDxgiAdapter(DXGIAdapter dxgiAdapter);
	static void getDefaultDxgiFactory(DXGIFactory dxgiFactory);

	static ComPtr<ID3D11Device> Device;
	static ComPtr<ID3D11DeviceContext> DeviceContext;
};

