#pragma once

#include "D3D11Ptr.h"
#include "D3D11Desc.h"

class DepthStencilState : public D3D11Ptr<ID3D11DepthStencilState>, public D3D11Desc<D3D11_DEPTH_STENCIL_DESC>
{
public:
	DepthStencilState();
	virtual ~DepthStencilState();

	void create();

	void setDepthEnable(BOOL enable);
	void setDepthWriteMask(D3D11_DEPTH_WRITE_MASK mask);
	void setDepthComparisonFunc(D3D11_COMPARISON_FUNC func);
	void setStencilEnable(BOOL enable);
	void setStencilReadMask(UINT8 mask);
	void setStencilWriteMask(UINT8 mask);
	void setFrontFaceStencilFailOp(D3D11_STENCIL_OP op);
	void setFrontFaceStencilDepthFailOp(D3D11_STENCIL_OP op);
	void setFrontFaceStencilPassOp(D3D11_STENCIL_OP op);
	void setFrontFaceStencilComparisonFunc(D3D11_COMPARISON_FUNC func);
	void setBackFaceStencilFailOp(D3D11_STENCIL_OP op);
	void setBackFaceStencilDepthFailOp(D3D11_STENCIL_OP op);
	void setBackFaceStencilPassOp(D3D11_STENCIL_OP op);
	void setBackFaceStencilComparisonFunc(D3D11_COMPARISON_FUNC func);

	void initFast(bool enable, bool writable);

	void initDepthNone();
	void initDepthDefault();
	void initDepthRead();
};

