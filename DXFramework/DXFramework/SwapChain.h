#pragma once

#include "D3D11Ptr.h"
#include "D3D11Desc.h"

class SwapChain : public D3D11Ptr<IDXGISwapChain>, public D3D11Desc<DXGI_SWAP_CHAIN_DESC>
{
public:
	SwapChain();
	virtual ~SwapChain();

	void create();

	void setBufferCount(UINT count);
	void setBufferWidth(UINT width);
	void setBufferHeight(UINT height);
	void setBufferFormat(DXGI_FORMAT format);
	void setRefreshRateNumerator(UINT numerator);
	void setRefreshRateDenominator(UINT denominator);
	void setBufferScaling(DXGI_MODE_SCALING scaling);
	void setBufferScanlineOrdering(DXGI_MODE_SCANLINE_ORDER order);
	void setBufferUsage(DXGI_USAGE usage);
	void setFlags(UINT flags);
	void setOutputWindow(HWND hwnd);
	void setSampleCount(UINT count);
	void setSampleQuality(UINT quality);
	void setSwapEffect(DXGI_SWAP_EFFECT effect);
	void setWindowed(BOOL windowed);

	void resizeBuffers(UINT width, UINT height);
	void getBackBuffer(ID3D11Texture2D **backBuffer);
	void present(UINT syncInterval, UINT flags);
};

