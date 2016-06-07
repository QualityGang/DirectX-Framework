#pragma once

#include <unordered_map>

#include "NonCopyable.h"
#include "Font.h"
#include "Rect.h"

#define FA_ALLLOWERCASE "abcdefghijklmnopqrstuvwxyz"
#define FA_ALLUPPERCASE "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
#define FA_ALLNUMBERS   "1234567890"
//#define FA_ALLSYMBOLS   ""

class FontAtlas : public NonCopyable
{
public:
	FontAtlas(LPCSTR chars, const Font *font);
	virtual ~FontAtlas();

	struct CharInfo
	{
		XMUINT2 advance;
		Rect<INT, UINT> rect;
		int ascent;
		int descent;
		XMUINT2 charPos;
	};

	const CharInfo* getCharInfo(char c) const;

	const Font* getFont() const { return font; }
	UINT getWidth() const { return width; }
	UINT getHeight() const { return height; }
	void getShaderResourceView(ID3D11ShaderResourceView **srv) const { *srv = this->srv; }
private:
	static bool LoadChar(FT_Face face, FT_ULong charCode);

	const Font *font;
	UINT width;
	UINT height;
	ID3D11ShaderResourceView *srv;
	mutable std::unordered_map<char, CharInfo> charMap;
};
