#pragma once

#include "D3D11Ptr.h"
#include "D3D11Desc.h"
#include "D3D11SubResData.h"

class Buffer : public D3D11SubResData, public D3D11Ptr<ID3D11Buffer>, public D3D11Desc<D3D11_BUFFER_DESC>
{
public:
	Buffer();
	virtual ~Buffer();

	void create();

	void setByteWidth(UINT width);
	void setUsage(D3D11_USAGE usage);
	void setBindFlags(UINT flags);
	void setCPUAccessFlags(UINT flags);
	void setMiscFlags(UINT flags);
	void StructureByteStride(UINT stride);
};

