#include "stdafx.h"
#include "InputLayout.h"



InputLayout::InputLayout()
{
}

InputLayout::~InputLayout()
{
}

void InputLayout::create(const Shader &shader, Element *elements, UINT count)
{
	
}

void InputLayout::Element::setSemanticName(LPCSTR name)
{
	desc.SemanticName = name;
}

void InputLayout::Element::setSemanticIndex(UINT semanticIndex)
{
	desc.SemanticIndex = semanticIndex;
}

void InputLayout::Element::setFormat(DXGI_FORMAT format)
{
	desc.Format = format;
}

void InputLayout::Element::setInputSlot(UINT slot)
{
	desc.InputSlot = slot;
}

void InputLayout::Element::setAlignedByteOffset(UINT offset)
{
	desc.AlignedByteOffset = offset;
}

void InputLayout::Element::setInputSlotClass(D3D11_INPUT_CLASSIFICATION cls)
{
	desc.InputSlotClass = cls;
}

void InputLayout::Element::setInstanceDataStepRate(UINT rate)
{
	desc.InstanceDataStepRate = rate;
}