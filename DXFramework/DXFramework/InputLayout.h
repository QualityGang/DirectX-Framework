#pragma once

#include "D3D11Ptr.h"
#include "D3D11Desc.h"
#include "Shader.h"

class InputLayout : public D3D11Ptr<ID3D11InputLayout>
{
public:
	InputLayout();
	virtual ~InputLayout();

	class Element : public D3D11Desc<D3D11_INPUT_ELEMENT_DESC>
	{
	public:
		void setSemanticName(LPCSTR name);
		void setSemanticIndex(UINT semanticIndex);
		void setFormat(DXGI_FORMAT format);
		void setInputSlot(UINT slot);
		void setAlignedByteOffset(UINT offset);
		void setInputSlotClass(D3D11_INPUT_CLASSIFICATION cls);
		void setInstanceDataStepRate(UINT rate);
	};

	void create(const Shader &shader, Element *elements, UINT count);
};

