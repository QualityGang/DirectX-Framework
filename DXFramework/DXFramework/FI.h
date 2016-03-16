#pragma once

#include "Bitmap.h"

class FI
{
public:
	FI() = delete;
	~FI() = delete;

	static bool CreateBitmap(LPCSTR filename, Bitmap *bitmap);
private:
	class StaticInit
	{
	public:
		StaticInit();
		virtual ~StaticInit();
	};

	static StaticInit __static_init;
};