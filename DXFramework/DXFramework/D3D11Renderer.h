#pragma once
class D3D11Renderer
{
public:
	D3D11Renderer() = delete;
	~D3D11Renderer() = delete;

	static ID3D11Device *Device;
	static ID3D11DeviceContext *DeviceContext;

	static void CreateDevice(ID3D11Device **device, ID3D11DeviceContext **immContext);
	static void CreateSwapChain(HWND hwnd, UINT width, UINT height, IDXGISwapChain **swapChain);
	static void CreateRenderTargetView(ID3D11Resource *res, ID3D11RenderTargetView **rtv);
	static void CreateRenderTargetView(IDXGISwapChain *swapChain, ID3D11RenderTargetView **rtv);
	static void CreateDepthStencilView(UINT width, UINT height, UINT sampleCount, UINT sampleQuality, ID3D11DepthStencilView **dsv);
	static void CreateTexture2D(UINT width, UINT height, DXGI_FORMAT format, UINT bindFlags, D3D11_USAGE usage, UINT sampleCount, UINT sampleQuality, void *initData, UINT rowWidth, ID3D11Texture2D **tex);
	static void CreateShaderResourceView(ID3D11Resource *res, ID3D11ShaderResourceView **srv);
	static void CreateVertexShader(const void *bytecode, UINT len, ID3D11VertexShader **vs);
	static void CreatePixelShader(const void *bytecode, UINT len, ID3D11PixelShader **ps);
	static void CreateBuffer(D3D11_BIND_FLAG bindFlag, UINT size, D3D11_USAGE usage, void *initData, ID3D11Buffer **buffer);
	static void CreateInputLayout(const void *bytecode, UINT bLen, D3D11_INPUT_ELEMENT_DESC *elements, UINT eLen, ID3D11InputLayout **inLayout);

	static void CreateBS(D3D11_BLEND srcBlend, D3D11_BLEND dstBlend, D3D11_BLEND_OP blendOp, ID3D11BlendState **bs);
	static void CreateBSOpaque(ID3D11BlendState **bs);
	static void CreateBSAlphaBlend(ID3D11BlendState **bs);
	static void CreateBSAdditive(ID3D11BlendState **bs);
	static void CreateBSPremultiplied(ID3D11BlendState **bs);

	static void CreateDSS(bool enable, bool writable, ID3D11DepthStencilState **dss);
	static void CreateDSSDepthNone(ID3D11DepthStencilState **dss);
	static void CreateDSSDepthDefault(ID3D11DepthStencilState **dss);
	static void CreateDSSDepthRead(ID3D11DepthStencilState **dss);

	static void CreateRS(D3D11_CULL_MODE cullMode, D3D11_FILL_MODE fillMode, ID3D11RasterizerState **rs);
	static void CreateRSCullNone(ID3D11RasterizerState **rs);
	static void CreateRSCullClockwise(ID3D11RasterizerState **rs);
	static void CreateRSCullCounterClockwise(ID3D11RasterizerState **rs);
	static void CreateRSWireframe(ID3D11RasterizerState **rs);

	static void CreateSS(D3D11_FILTER filter, D3D11_TEXTURE_ADDRESS_MODE addressMode, UINT maxAnisotropy, ID3D11SamplerState **ss);
	static void CreateSSPointWrap(ID3D11SamplerState **ss);
	static void CreateSSPointClamp(ID3D11SamplerState **ss);
	static void CreateSSLinearWrap(ID3D11SamplerState **ss);
	static void CreateSSLinearClamp(ID3D11SamplerState **ss);
	static void CreateSSAnisotropicWrap(UINT maxAnisotropy, ID3D11SamplerState **ss);
	static void CreateSSAnisotropicClamp(UINT maxAnisotropy, ID3D11SamplerState **ss);

	static void GetDXGIDevice(IDXGIDevice **dxgiDevice);
	static void GetDXGIAdapter(IDXGIAdapter **dxgiAdapter);
	static void GetDXGIFactory(IDXGIFactory **dxgiFactory);

	static void ResizeSwapChain(IDXGISwapChain *swapChain, UINT width, UINT height);
	static void SetUsage(D3D11_USAGE *descUsage, UINT *descCpuFlags, D3D11_USAGE usage);

	static void MakeWindowAssociation(HWND hwmd, UINT flag);
	static void DisableAltEnter(HWND hwnd);
	static void DisableWindowChanges(HWND hwnd);
	static void DisablePrintScreen(HWND hwnd);

	static void Map(ID3D11Resource *res, D3D11_MAP mapType, void **ptr, UINT *rowWidth = nullptr);
	static void Unmap(ID3D11Resource *res);

	static void GetTexture2D(ID3D11ShaderResourceView *srv, ID3D11Texture2D **tex);
	static void GetTexture2DSize(ID3D11Texture2D *tex, XMFLOAT2 *size);
	static void GetTexture2DSize(ID3D11ShaderResourceView *srv, XMFLOAT2 *size);
private:
	class StaticInit
	{
	public:
		StaticInit();
		virtual ~StaticInit();
	};

	static StaticInit __static_init;
};
