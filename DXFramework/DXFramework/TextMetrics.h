#pragma once

#include "SpriteBatch.h"
#include "FontAtlas.h"

//class Text;

class TextMetrics
{
public:
	TextMetrics() = delete;
	~TextMetrics() = delete;

	static int GetMaxAscent(LPCSTR str, const FontAtlas &atlas);
	static int GetMaxDescent(LPCSTR str, const FontAtlas &atlas);
	static void GetSize(const Text &text, const FontAtlas &atlas, XMUINT2 *size);
};

