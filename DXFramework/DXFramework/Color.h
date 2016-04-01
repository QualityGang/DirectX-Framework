#pragma once

template <typename T>
struct Color
{
	Color() {}
	Color(T r, T g, T b, T a)
		: r(r), g(g), b(b), a(a) {}
	virtual ~Color() {}

	void set(T r, T g, T b, T a)
	{
		this->r = r;
		this->g = g;
		this->b = b;
		this->a = a;
	}

	T r, g, b, a;
};