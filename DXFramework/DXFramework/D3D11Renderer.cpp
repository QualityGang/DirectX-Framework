#include "stdafx.h"
#include "D3D11Renderer.h"

#include "Settings.h"


D3D11Renderer::StaticInit D3D11Renderer::__static_init;

ID3D11Device* D3D11Renderer::Device;
ID3D11DeviceContext* D3D11Renderer::DeviceContext;

D3D11Renderer::StaticInit::StaticInit()
{
	CreateDevice(&Device, &DeviceContext);
}

D3D11Renderer::StaticInit::~StaticInit()
{
	SafeRelease(Device);
	SafeRelease(DeviceContext);
}

void D3D11Renderer::CreateSwapChain(HWND hwnd, UINT width, UINT height, IDXGISwapChain **swapChain)
{
	DXGI_SWAP_CHAIN_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.BufferCount = BUFFER_COUNT;
	desc.BufferDesc.Width = width;
	desc.BufferDesc.Height = height;
	desc.BufferDesc.Format = BUFFER_FORMAT;
	desc.BufferDesc.RefreshRate.Numerator = 60;
	desc.BufferDesc.RefreshRate.Denominator = 1;
	desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	desc.OutputWindow = hwnd;
	desc.SampleDesc.Count = SAMPLE_COUNT;
	desc.SampleDesc.Quality = SAMPLE_QUALITY;
	desc.Flags = SWAPCHAIN_FLAGS;
	desc.Windowed = TRUE;

	IDXGIFactory *dxgiFactory;
	GetDXGIFactory(&dxgiFactory);

	HRESULT hr = dxgiFactory->CreateSwapChain(Device, &desc, swapChain);
	SafeRelease(dxgiFactory);

	if (FAILED(hr))
	{
		PrintError(hr);
		*swapChain = nullptr;
	}
}

void D3D11Renderer::CreateRenderTargetView(ID3D11Resource *res, ID3D11RenderTargetView **rtv)
{
	HRESULT hr = Device->CreateRenderTargetView(res, nullptr, rtv);

	if (FAILED(hr))
	{
		PrintError(hr);
		*rtv = nullptr;
	}
}

void D3D11Renderer::CreateRenderTargetView(IDXGISwapChain *swapChain, ID3D11RenderTargetView **rtv)
{
	ID3D11Texture2D *backBuffer = nullptr;
	HRESULT hr = swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBuffer);

	if (FAILED(hr))
	{
		PrintError(hr);
		*rtv = nullptr;
		return;
	}

	CreateRenderTargetView(backBuffer, rtv);
	SafeRelease(backBuffer);
}

void D3D11Renderer::CreateDepthStencilView(UINT width, UINT height, ID3D11DepthStencilView **dsv)
{
	D3D11_TEXTURE2D_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.ArraySize = 1;
	desc.MipLevels = 1;
	desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	desc.Width = width;
	desc.Height = height;
	desc.SampleDesc.Count = SAMPLE_COUNT;
	desc.SampleDesc.Quality = SAMPLE_QUALITY;

	ID3D11Texture2D *buffer;
	HRESULT hr = Device->CreateTexture2D(&desc, nullptr, &buffer);

	if (FAILED(hr))
	{
		PrintError(hr);
		*dsv = nullptr;
		return;
	}

	hr = Device->CreateDepthStencilView(buffer, nullptr, dsv);
	SafeRelease(buffer);

	if (FAILED(hr))
	{
		PrintError(hr);
		*dsv = nullptr;
	}
}

void D3D11Renderer::CreateTexture2D(UINT width, UINT height, DXGI_FORMAT format, UINT bindFlags, D3D11_USAGE usage, void *initData, UINT rowWidth, ID3D11Texture2D **tex)
{
	D3D11_TEXTURE2D_DESC desc;
	desc.Width = width;
	desc.Height = height;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = format;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.BindFlags = bindFlags;
	desc.MiscFlags = 0;

	SetUsage(&desc.Usage, &desc.CPUAccessFlags, usage);

	HRESULT hr;

	if (initData)
	{
		D3D11_SUBRESOURCE_DATA resData;
		ZeroMemory(&resData, sizeof(resData));
		resData.pSysMem = initData;
		resData.SysMemPitch = rowWidth;

		hr = Device->CreateTexture2D(&desc, &resData, tex);
	}
	else hr = Device->CreateTexture2D(&desc, nullptr, tex);

	if (FAILED(hr))
	{
		PrintError(hr);
		*tex = nullptr;
	}
}

void D3D11Renderer::CreateShaderResourceView(ID3D11Resource *res, ID3D11ShaderResourceView **srv)
{
	HRESULT hr = Device->CreateShaderResourceView(res, nullptr, srv);

	if (FAILED(hr))
	{
		PrintError(hr);
		*srv = nullptr;
	}
}

void D3D11Renderer::CreateVertexShader(const void *bytecode, UINT len, ID3D11VertexShader **vs)
{
	HRESULT hr = Device->CreateVertexShader(bytecode, len, nullptr, vs);

	if (FAILED(hr))
	{
		PrintError(hr);
		*vs = nullptr;
	}
}

void D3D11Renderer::CreatePixelShader(const void * bytecode, UINT len, ID3D11PixelShader **ps)
{
	HRESULT hr = Device->CreatePixelShader(bytecode, len, nullptr, ps);

	if (FAILED(hr))
	{
		PrintError(hr);
		*ps = nullptr;
	}
}

void D3D11Renderer::CreateBuffer(D3D11_BIND_FLAG bindFlag, UINT size, D3D11_USAGE usage, void *initData, ID3D11Buffer **buffer)
{
	D3D11_BUFFER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.BindFlags = bindFlag;
	desc.ByteWidth = size;

	SetUsage(&desc.Usage, &desc.CPUAccessFlags, usage);

	HRESULT hr;

	if (initData)
	{
		D3D11_SUBRESOURCE_DATA resData;
		ZeroMemory(&resData, sizeof(resData));
		resData.pSysMem = initData;

		hr = Device->CreateBuffer(&desc, &resData, buffer);
	}
	else hr = Device->CreateBuffer(&desc, nullptr, buffer);

	if (FAILED(hr))
	{
		PrintError(hr);
		*buffer = nullptr;
	}
}

void D3D11Renderer::CreateInputLayout(const void *bytecode, UINT bLen, D3D11_INPUT_ELEMENT_DESC *elements, UINT eLen, ID3D11InputLayout **inLayout)
{
	HRESULT hr = Device->CreateInputLayout(elements, eLen, bytecode, bLen, inLayout);

	if (FAILED(hr))
	{
		PrintError(hr);
		*inLayout = nullptr;
	}
}

void D3D11Renderer::CreateBS(D3D11_BLEND srcBlend, D3D11_BLEND dstBlend, D3D11_BLEND_OP blendOp, ID3D11BlendState **bs)
{
	D3D11_BLEND_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.RenderTarget[0].BlendEnable = (srcBlend != D3D11_BLEND_ONE) || (dstBlend != D3D11_BLEND_ZERO);
	desc.RenderTarget[0].SrcBlend = srcBlend;
	desc.RenderTarget[0].SrcBlendAlpha = srcBlend;
	desc.RenderTarget[0].DestBlend = dstBlend;
	desc.RenderTarget[0].DestBlendAlpha = dstBlend;
	desc.RenderTarget[0].BlendOp = blendOp;
	desc.RenderTarget[0].BlendOpAlpha = blendOp;
	desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	HRESULT hr = Device->CreateBlendState(&desc, bs);

	if (FAILED(hr))
	{
		PrintError(hr);
		*bs = nullptr;
	}
}

void D3D11Renderer::CreateBSOpaque(ID3D11BlendState **bs)
{
	CreateBS(D3D11_BLEND_ONE, D3D11_BLEND_ZERO, D3D11_BLEND_OP_ADD, bs);
}

void D3D11Renderer::CreateBSAlphaBlend(ID3D11BlendState **bs)
{
	CreateBS(D3D11_BLEND_SRC_ALPHA, D3D11_BLEND_INV_SRC_ALPHA, D3D11_BLEND_OP_ADD, bs);
}

void D3D11Renderer::CreateBSAdditive(ID3D11BlendState **bs)
{
	CreateBS(D3D11_BLEND_SRC_ALPHA, D3D11_BLEND_ONE, D3D11_BLEND_OP_ADD, bs);
}

void D3D11Renderer::CreateBSPremultiplied(ID3D11BlendState ** bs)
{
	CreateBS(D3D11_BLEND_ONE, D3D11_BLEND_INV_SRC_ALPHA, D3D11_BLEND_OP_ADD, bs);
}

void D3D11Renderer::CreateDSS(bool enable, bool writable, ID3D11DepthStencilState **dss)
{
	D3D11_DEPTH_STENCIL_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.DepthEnable = enable;
	desc.DepthWriteMask = writable ? D3D11_DEPTH_WRITE_MASK_ALL : D3D11_DEPTH_WRITE_MASK_ZERO;
	desc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
	desc.StencilEnable = false;
	//desc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
	//desc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;
	//desc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	//desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	//desc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	//desc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	//desc.BackFace = depthStencilStateDesc.FrontFace;

	HRESULT hr = Device->CreateDepthStencilState(&desc, dss);

	if (FAILED(hr))
	{
		PrintError(hr);
		*dss = nullptr;
	}
}

void D3D11Renderer::CreateDSSDepthNone(ID3D11DepthStencilState **dss)
{
	CreateDSS(false, false, dss);
}

void D3D11Renderer::CreateDSSDepthDefault(ID3D11DepthStencilState **dss)
{
	CreateDSS(true, true, dss);
}

void D3D11Renderer::CreateDSSDepthRead(ID3D11DepthStencilState **dss)
{
	CreateDSS(true, false, dss);
}

void D3D11Renderer::CreateRS(D3D11_CULL_MODE cullMode, D3D11_FILL_MODE fillMode, ID3D11RasterizerState **rs)
{
	D3D11_RASTERIZER_DESC desc;
	ZeroMemory(&desc, sizeof(D3D11_RASTERIZER_DESC));
	desc.CullMode = cullMode;
	desc.FillMode = fillMode;
	desc.DepthClipEnable = TRUE;
	//desc.MultisampleEnable = TRUE;

	HRESULT hr = Device->CreateRasterizerState(&desc, rs);

	if (FAILED(hr))
	{
		PrintError(hr);
		*rs = nullptr;
	}
}

void D3D11Renderer::CreateRSCullNone(ID3D11RasterizerState **rs)
{
	CreateRS(D3D11_CULL_NONE, D3D11_FILL_SOLID, rs);
}

void D3D11Renderer::CreateRSCullClockwise(ID3D11RasterizerState **rs)
{
	CreateRS(D3D11_CULL_FRONT, D3D11_FILL_SOLID, rs);
}

void D3D11Renderer::CreateRSCullCounterClockwise(ID3D11RasterizerState **rs)
{
	CreateRS(D3D11_CULL_BACK, D3D11_FILL_SOLID, rs);
}

void D3D11Renderer::CreateRSWireframe(ID3D11RasterizerState **rs)
{
	CreateRS(D3D11_CULL_NONE, D3D11_FILL_WIREFRAME, rs);
}

void D3D11Renderer::CreateSS(D3D11_FILTER filter, D3D11_TEXTURE_ADDRESS_MODE addressMode, UINT maxAnisotropy, ID3D11SamplerState **ss)
{
	D3D11_SAMPLER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.Filter = filter;
	desc.AddressU = addressMode;
	desc.AddressV = addressMode;
	desc.AddressW = addressMode;
	desc.MaxAnisotropy = maxAnisotropy;
	desc.MaxLOD = D3D11_FLOAT32_MAX;
	desc.ComparisonFunc = D3D11_COMPARISON_NEVER;

	HRESULT hr = Device->CreateSamplerState(&desc, ss);

	if (FAILED(hr))
	{
		PrintError(hr);
		*ss = nullptr;
	}
}

void D3D11Renderer::CreateSSPointWrap(ID3D11SamplerState **ss)
{
	CreateSS(D3D11_FILTER_MIN_MAG_MIP_POINT, D3D11_TEXTURE_ADDRESS_WRAP, 0, ss);
}

void D3D11Renderer::CreateSSPointClamp(ID3D11SamplerState **ss)
{
	CreateSS(D3D11_FILTER_MIN_MAG_MIP_POINT, D3D11_TEXTURE_ADDRESS_CLAMP, 0, ss);
}

void D3D11Renderer::CreateSSLinearWrap(ID3D11SamplerState **ss)
{
	CreateSS(D3D11_FILTER_MIN_MAG_MIP_LINEAR, D3D11_TEXTURE_ADDRESS_WRAP, 0, ss);
}

void D3D11Renderer::CreateSSLinearClamp(ID3D11SamplerState **ss)
{
	CreateSS(D3D11_FILTER_MIN_MAG_MIP_LINEAR, D3D11_TEXTURE_ADDRESS_CLAMP, 0, ss);
}

void D3D11Renderer::CreateSSAnisotropicWrap(UINT maxAnisotropy, ID3D11SamplerState **ss)
{
	CreateSS(D3D11_FILTER_ANISOTROPIC, D3D11_TEXTURE_ADDRESS_WRAP, maxAnisotropy, ss);
}

void D3D11Renderer::CreateSSAnisotropicClamp(UINT maxAnisotropy, ID3D11SamplerState **ss)
{
	CreateSS(D3D11_FILTER_ANISOTROPIC, D3D11_TEXTURE_ADDRESS_CLAMP, maxAnisotropy, ss);
}

void D3D11Renderer::CreateDevice(ID3D11Device **device, ID3D11DeviceContext **immContext)
{
	UINT createDeviceFlags = 0;
#if _DEBUG
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_0;

	HRESULT hr = D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr,
								   createDeviceFlags, &featureLevel, 1, D3D11_SDK_VERSION,
								   device, nullptr, immContext);

	if (FAILED(hr))
	{
		PrintError(hr);
		*device = nullptr;
		*immContext = nullptr;
	}
}

void D3D11Renderer::SetUsage(D3D11_USAGE *descUsage, UINT *descCpuFlags, D3D11_USAGE usage)
{
	*descUsage = usage;

	switch (usage)
	{
	case D3D11_USAGE_DEFAULT:
	case D3D11_USAGE_IMMUTABLE:
		*descCpuFlags = 0;
		break;
	case D3D11_USAGE_DYNAMIC:
		*descCpuFlags = D3D11_CPU_ACCESS_WRITE;
		break;
	case D3D11_USAGE_STAGING:
		*descCpuFlags = D3D11_CPU_ACCESS_WRITE | D3D11_CPU_ACCESS_READ;
		break;
	default:
		error_stream << "Unknown usage flag" << std::endl;
	}
}

void D3D11Renderer::GetDXGIDevice(IDXGIDevice **dxgiDevice)
{
	HRESULT hr = Device->QueryInterface(__uuidof(IDXGIDevice), (void**)dxgiDevice);

	if (FAILED(hr))
	{
		PrintError(hr);
		*dxgiDevice = nullptr;
	}
}

void D3D11Renderer::GetDXGIAdapter(IDXGIAdapter **dxgiAdapter)
{
	IDXGIDevice *dxgiDevice;
	GetDXGIDevice(&dxgiDevice);

	HRESULT hr = dxgiDevice->GetAdapter(dxgiAdapter);
	SafeRelease(dxgiDevice);

	if (FAILED(hr))
	{
		PrintError(hr);
		*dxgiAdapter = nullptr;
	}
}

void D3D11Renderer::GetDXGIFactory(IDXGIFactory **dxgiFactory)
{
	IDXGIAdapter *dxgiAdapter;
	GetDXGIAdapter(&dxgiAdapter);

	HRESULT hr = dxgiAdapter->GetParent(__uuidof(IDXGIFactory), (void**)dxgiFactory);
	SafeRelease(dxgiAdapter);

	if (FAILED(hr))
	{
		PrintError(hr);
		*dxgiFactory = nullptr;
	}
}

void D3D11Renderer::ResizeSwapChain(IDXGISwapChain *swapChain, UINT width, UINT height)
{
	HRESULT hr = swapChain->ResizeBuffers(BUFFER_COUNT, width, height, BUFFER_FORMAT, SWAPCHAIN_FLAGS);

	if (FAILED(hr))
	{
		PrintError(hr);
	}
}

void D3D11Renderer::DisableAltEnter(HWND hwnd)
{
	MakeWindowAssociation(hwnd, DXGI_MWA_NO_ALT_ENTER);
}

void D3D11Renderer::DisableWindowChanges(HWND hwnd)
{
	MakeWindowAssociation(hwnd, DXGI_MWA_NO_WINDOW_CHANGES);
}

void D3D11Renderer::DisablePrintScreen(HWND hwnd)
{
	MakeWindowAssociation(hwnd, DXGI_MWA_NO_PRINT_SCREEN);
}

void D3D11Renderer::Map(ID3D11Resource *res, D3D11_MAP mapType, void **ptr, UINT *rowWidth)
{
	D3D11_MAPPED_SUBRESOURCE mappedRes;
	HRESULT hr = DeviceContext->Map(res, 0, mapType, 0, &mappedRes);

	if (FAILED(hr))
	{
		PrintError(hr);
		*ptr = nullptr;
		*rowWidth = 0;
		return;
	}

	*ptr = mappedRes.pData;

	if (rowWidth) 
		*rowWidth = mappedRes.RowPitch;
}

void D3D11Renderer::Unmap(ID3D11Resource *res)
{
	DeviceContext->Unmap(res, 0);
}

void D3D11Renderer::MakeWindowAssociation(HWND hwnd, UINT flag)
{
	IDXGIFactory *dxgiFactory;
	GetDXGIFactory(&dxgiFactory);

	HRESULT hr = dxgiFactory->MakeWindowAssociation(hwnd, flag);
	SafeRelease(dxgiFactory);

	if (FAILED(hr))
	{
		PrintError(hr);
	}
}