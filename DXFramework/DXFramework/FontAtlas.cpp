#include "stdafx.h"
#include "FontAtlas.h"

#include "Bitmap.h"

#include <algorithm>
#include <unordered_set>

#define GAP_SIZE 5 

FontAtlas::FontAtlas(LPCSTR chars, const Font *font) :
	font(font), width(GAP_SIZE)
{
	FT_Face face = font->getFace();
	FT_GlyphSlot glyph = face->glyph;

	width += glyph->bitmap.width + GAP_SIZE;
	height =  glyph->bitmap.rows;

	std::unordered_set<char> charSet(chars, chars + strlen(chars));
	charSet.insert(' ');

	for (const char c : charSet)
	{
		if (!LoadChar(face, c))
		{
			error_stream << "LoadChar failed " << c << std::endl;
			continue;
		}

		width += glyph->bitmap.width + GAP_SIZE;
		height = std::max(height, glyph->bitmap.rows);
	}

	height += 2 * GAP_SIZE;

	UINT x = GAP_SIZE;
	UINT y = GAP_SIZE;

	Bitmap bmp(width, height);

	for (const char c : charSet)
	{
		if (!LoadChar(face, c))
		{
			error_stream << "LoadChar failed " << c << std::endl;
			continue;
		}

		for (UINT iy = 0; iy < glyph->bitmap.rows; iy++)
		{
			for (UINT ix = 0; ix < glyph->bitmap.width; ix++)
			{
				UINT index = iy * glyph->bitmap.pitch + ix;
				bmp.setPixel(x + ix, y + iy, 255, 255, 255, glyph->bitmap.buffer[index]);
			}
		}

		CharInfo charInfo;
		charInfo.advance.x = glyph->advance.x >> 6;
		charInfo.advance.y = glyph->advance.y >> 6;
		charInfo.rect.x = glyph->bitmap_left;
		charInfo.rect.y = glyph->bitmap_top;
		charInfo.rect.width  = glyph->bitmap.width;
		charInfo.rect.height = glyph->bitmap.rows;
		charInfo.ascent  = glyph->metrics.horiBearingY >> 6;
		charInfo.descent = (glyph->metrics.height - glyph->metrics.horiBearingY) >> 6;
		charInfo.charPos.x = x;
		charInfo.charPos.y = y;

		charMap[c] = charInfo;
		x += glyph->bitmap.width + GAP_SIZE;
	}

	if (!getCharInfo(' '))
		error_stream << "space char not found" << std::endl;

	bmp.getShaderResourceView(&srv);
	SafeAddRef(srv);
}

FontAtlas::~FontAtlas()
{
	SafeRelease(srv);
}

const FontAtlas::CharInfo* FontAtlas::getCharInfo(char c) const
{
	auto iter = charMap.find(c);

	if (iter == charMap.end())
		return nullptr;

	return &iter->second;
}

bool FontAtlas::LoadChar(FT_Face face, FT_ULong charCode)
{
	FT_Error error = FT_Load_Char(face, charCode, FT_LOAD_RENDER);

	if (error)
	{
		error_stream << "Failed to load char " << charCode << " " << (char)charCode << std::endl;
		return false;
	}

	return true;
}