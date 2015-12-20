#pragma once

template <typename T>
class D3D11Ptr
{
public:
	typedef ComPtr<T> Ptr;
	typedef T* RawPtr;

	//bool isValid() const;

	RawPtr getPtr() const;
protected:
	D3D11Ptr();
	virtual ~D3D11Ptr();

	Ptr ptr;
};

#include "D3D11Ptr.inl"
