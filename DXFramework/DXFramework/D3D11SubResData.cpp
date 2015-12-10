#include "stdafx.h"
#include "D3D11SubResData.h"


D3D11SubResData::D3D11SubResData()
{
	ZeroMemory(&resData, sizeof(resData));
}

D3D11SubResData::~D3D11SubResData()
{
}

void D3D11SubResData::setMem(void *initData)
{
	resData.pSysMem = initData;
}

void D3D11SubResData::setMemPitch(UINT pitch)
{
	resData.SysMemPitch = pitch;
}

void D3D11SubResData::setMemSlicePitch(UINT slicePitch)
{
	resData.SysMemSlicePitch = slicePitch;
}
