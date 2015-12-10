#pragma once 

template <typename T>
class D3D11Desc
{
public:
	typedef T Desc;

	const Desc& getDesc() const;
protected:
	D3D11Desc();
	virtual ~D3D11Desc();

	Desc desc;
};

#include "D3D11Desc.inl"