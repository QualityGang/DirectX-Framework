#pragma once
class D3D11SubResData
{
public:
	void setMem(void *initData);
	void setMemPitch(UINT pitch);
	void setMemSlicePitch(UINT slicePitch);
protected:
	D3D11SubResData();
	virtual ~D3D11SubResData();

	D3D11_SUBRESOURCE_DATA resData;
};

