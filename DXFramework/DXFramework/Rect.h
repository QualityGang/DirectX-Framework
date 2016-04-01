#pragma once

template <typename T1, typename T2 = T1>
struct Rect
{
	Rect() {}
	Rect(T1 x, T1 y, T2 width, T2 height)
		: x(x), y(y), width(width), height(height) {}
	virtual ~Rect() {}

	void set(T1 x, T1 y, T2 width, T2 height)
	{
		this->x = x;
		this->y = y;
		this->width = width;
		this->height = height;
	}

	T1 x, y;
	T2 width, height;
};