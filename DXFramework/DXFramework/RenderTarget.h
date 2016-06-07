#pragma once

#include "NonCopyable.h"
#include "AlignedNew.h"

ALIGN(16)
class RenderTarget : public NonCopyable, public AlignedNew<16>
{
public:
	RenderTarget();
	virtual ~RenderTarget();

	void clear(float r, float g, float b, float a);
	void updateSize(float width, float height);

	ID3D11RenderTargetView* getRenderTargetView() const;
	ID3D11DepthStencilView* getDepthStencilView() const;
	const XMMATRIX& getProjectionMatrix() const;
	const D3D11_VIEWPORT& getViewport() const;
protected:	
	ID3D11RenderTargetView *renderTargetView = nullptr;
	ID3D11DepthStencilView *depthStencilView = nullptr;
private:
	XMMATRIX projectionMatrix;
	D3D11_VIEWPORT viewport;
};

