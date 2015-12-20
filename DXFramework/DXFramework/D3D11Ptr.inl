


template <typename T>
D3D11Ptr<T>::D3D11Ptr()
{
}

template <typename T>
D3D11Ptr<T>::~D3D11Ptr()
{
}

//template <typename T>
//bool D3D11Ptr<T>::isValid() const
//{
//	return ptr != nullptr;
//}

template <typename T>
typename D3D11Ptr<T>::RawPtr D3D11Ptr<T>::getPtr() const
{
	return ptr.Get();
}