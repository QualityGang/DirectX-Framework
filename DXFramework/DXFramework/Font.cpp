#include "stdafx.h"
#include "Font.h"


Font::Font(LPCSTR filename, USHORT size)
{
	FT_Error error = FT_New_Face(FT::Library, filename, 0, &face);

	if (error)
	{
		error_stream << "Failed to create font " << filename << std::endl;
		face = nullptr;
	}

	setSize(size);
}

Font::~Font()
{
	FT_Done_Face(face);
}

void Font::setSize(UINT size)
{
	if (size != getSize())
	{
		FT_Error error = FT_Set_Pixel_Sizes(face, 0, size);

		if (error)
			error_stream << "Failed to set font size " << size << std::endl;
	}
}

int Font::getKerning(UINT leftIndex, UINT rightIndex) const
{
	FT_Bool useKerning = FT_HAS_KERNING(face);

	if (useKerning && leftIndex && rightIndex)
	{
		FT_Vector delta;
		FT_Error error = FT_Get_Kerning(face, leftIndex, rightIndex, FT_KERNING_DEFAULT, &delta);

		if (!error)
			return delta.x >> 6;
	}

	return 0;
}