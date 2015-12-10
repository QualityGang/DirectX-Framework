


template <typename T>
D3D11Desc<T>::D3D11Desc()
{
	ZeroMemory(&desc, sizeof(desc));
}

template <typename T>
D3D11Desc<T>::~D3D11Desc()
{
}

template <typename T>
const typename D3D11Desc<T>::Desc& D3D11Desc<T>::getDesc() const
{
	return desc;
}