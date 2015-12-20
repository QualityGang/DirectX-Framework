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
	HR(Pipeline::Device->CreateShaderResourceView(resource, nullptr, ptr.GetAddressOf()));
}
