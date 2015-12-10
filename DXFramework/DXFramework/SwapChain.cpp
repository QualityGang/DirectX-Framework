#include "stdafx.h"
#include "SwapChain.h"



SwapChain::SwapChain()
{
}

SwapChain::~SwapChain()
{
}

void SwapChain::create()
{
	
}

void SwapChain::setBufferCount(UINT count)
{
	desc.BufferCount = count;
}

void SwapChain::setBufferWidth(UINT width)
{
	desc.BufferDesc.Width = width;
}

void SwapChain::setBufferHeight(UINT height)
{
	desc.BufferDesc.Height = height;
}

void SwapChain::setBufferFormat(DXGI_FORMAT format)
{
	desc.BufferDesc.Format = format;
}

void SwapChain::setRefreshRateNumerator(UINT numerator)
{
	desc.BufferDesc.RefreshRate.Numerator = numerator;
}

void SwapChain::setRefreshRateDenominator(UINT denominator)
{
	desc.BufferDesc.RefreshRate.Denominator = denominator;
}

void SwapChain::setBufferScaling(DXGI_MODE_SCALING scaling)
{
	desc.BufferDesc.Scaling = scaling;
}

void SwapChain::setBufferScanlineOrdering(DXGI_MODE_SCANLINE_ORDER order)
{
	desc.BufferDesc.ScanlineOrdering = order;
}

void SwapChain::setBufferUsage(DXGI_USAGE usage)
{
	desc.BufferUsage = usage;
}

void SwapChain::setFlags(UINT flags)
{
	desc.Flags = flags;
}

void SwapChain::setOutputWindow(HWND hwnd)
{
	desc.OutputWindow = hwnd;
}

void SwapChain::setSampleCount(UINT count)
{
	desc.SampleDesc.Count = count;
}

void SwapChain::setSampleQuality(UINT quality)
{
	desc.SampleDesc.Quality = quality;
}

void SwapChain::setSwapEffect(DXGI_SWAP_EFFECT effect)
{
	desc.SwapEffect = effect;
}

void SwapChain::setWindowed(BOOL windowed)
{
	desc.Windowed = windowed;
}

void SwapChain::resizeBuffers(UINT width, UINT height)
{
	HR(ptr->ResizeBuffers(desc.BufferCount, width, height, desc.BufferDesc.Format, desc.Flags));
}

void SwapChain::getBackBuffer(ID3D11Texture2D **backBuffer)
{
	HR(ptr->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)backBuffer));
}

void SwapChain::present(UINT syncInterval, UINT flags)
{
	HR(ptr->Present(syncInterval, flags));
}