#include "stdafx.h"
#include "TextMetrics.h"

#include "SpriteBatch.h"
#include "string_ext.h"


int TextMetrics::GetMaxAscent(LPCSTR str, const FontAtlas &atlas)
{
	int ascent = 0;

	for (LPCSTR lpStr = str; *lpStr; lpStr++)
	{
		const FontAtlas::CharInfo *charInfo = atlas.getCharInfo(*lpStr);

		if (charInfo && charInfo->ascent > ascent)
			ascent = charInfo->ascent;
	}

	return ascent;
}

int TextMetrics::GetMaxDescent(LPCSTR str, const FontAtlas &atlas)
{
	int descent = 0;

	for (LPCSTR lpStr = str; *lpStr; lpStr++)
	{
		const FontAtlas::CharInfo *charInfo = atlas.getCharInfo(*lpStr);

		if (charInfo && charInfo->descent > descent)
			descent = charInfo->descent;
	}

	return descent;
}

void TextMetrics::GetSize(const Text &text, const FontAtlas &atlas, XMUINT2 *size)
{
	size->x = 0;
	size->y = 0;

	const Font *font = atlas.getFont();

	if (!font)
		return;

	FT_Face face = font->getFace();

	FT_UInt prevGlyphIndex = 0;
	XMFLOAT2 pen(0, 0);

	std::vector<std::string> lines;
	stde::string_split(text.str, "\n", &lines);

	UINT currLine = 0;
	int maxAscent = GetMaxAscent(lines[currLine].c_str(), atlas);
	int maxDescent = GetMaxDescent(lines[currLine].c_str(), atlas);

	for (LPCSTR lpStr = text.str; *lpStr; lpStr++)
	{
		switch (*lpStr)
		{
			case ' ':
				pen.x += atlas.getCharInfo(' ')->advance.x;
				continue;
			case '\t':
				pen.x += atlas.getCharInfo(' ')->advance.x * 3;
				continue;
			case '\n':
				pen.x = 0;
				pen.y += text.lineGap + maxDescent + maxAscent;
				currLine++;
				maxAscent = GetMaxAscent(lines[currLine].c_str(), atlas);
				maxDescent = GetMaxDescent(lines[currLine].c_str(), atlas);
				continue;
		}

		const FontAtlas::CharInfo *charInfo = atlas.getCharInfo(*lpStr);

		if (!charInfo)
		{
			error_stream << "Character missing from FontAtlas " << *lpStr << std::endl;
			pen.x += atlas.getCharInfo(' ')->advance.x;
			continue;
		}

		FT_UInt glyphIndex = FT_Get_Char_Index(face, *lpStr);
		pen.x += font->getKerning(prevGlyphIndex, glyphIndex);

		if (!*(lpStr + 1))
		{
			size->x = text.position.x + pen.x + charInfo->rect.x + charInfo->rect.width;
			size->y = text.position.y + pen.y + maxAscent - charInfo->rect.y + charInfo->rect.height;
		}
		else
		{
			pen.x += charInfo->advance.x;
			prevGlyphIndex = glyphIndex;
		}
	}
}
