#include "stdafx.h"
#include "ShaderResourceView.h"
#include "Pipeline.h"


ShaderResourceView::ShaderResourceView()
{
}

ShaderResourceView::~ShaderResourceView()
{
}

void ShaderResourceView::create(ID3D11Resource *resource)
{
	BF(Pipeline::Device.Get()->CreateShaderResourceView(resource, nullptr, ptr.GetAddressOf()));
}
