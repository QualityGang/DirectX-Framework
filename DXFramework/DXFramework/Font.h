#pragma once

#include "NonCopyable.h"
#include "FT.h"

class Font : public NonCopyable
{
public:
	Font(LPCSTR filename, USHORT size);
	virtual ~Font();

	void setSize(UINT size);

	FT_Face getFace() const { return face; }

	USHORT getSize() const { return face->size->metrics.x_ppem; }

	//int getLineSpacing() const
	//{ 
	//	return (face->size->metrics.height) >> 6;
	//}

	int getKerning(UINT leftIndex, UINT rightIndex) const;
private:
	FT_Face face;
};