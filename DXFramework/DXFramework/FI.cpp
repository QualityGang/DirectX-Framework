#include "stdafx.h"
#include "FI.h"



FI::StaticInit FI::__static_init;


FI::StaticInit::StaticInit()
{
#ifdef FREEIMAGE_LIB
#if VLD_ENABLED
	VLDDisable();
#endif
	FreeImage_Initialise();
#if VLD_ENABLED
	VLDEnable();
#endif
#endif
}

FI::StaticInit::~StaticInit()
{
#ifdef FREEIMAGE_LIB
#if VLD_ENABLED
	VLDDisable();
#endif
	FreeImage_DeInitialise();
#if VLD_ENABLED
	VLDEnable();
#endif
#endif
}

bool FI::CreateBitmap(LPCSTR filename, Bitmap *bitmap)
{
	FREE_IMAGE_FORMAT format = FreeImage_GetFileType(filename);

	if (format == FIF_UNKNOWN)
	{
		error_stream << "Unknown FI Format" << std::endl;
		return false;
	}

	FIBITMAP *image = nullptr;

	if (FreeImage_FIFSupportsReading(format))
		image = FreeImage_Load(format, filename);

	if (!image)
	{
		error_stream << "Failed to load FIBITMAP" << std::endl;
		return false;
	}

	if (!FreeImage_FlipVertical(image))
	{
		error_stream << "Failed to flip FIBITMAP" << std::endl;
		return false;
	}

	FIBITMAP *tmp = image;
	image = FreeImage_ConvertTo32Bits(image);
	FreeImage_Unload(tmp);

	if (!image)
	{
		error_stream << "Failed to convert FIBITMAP to 32bit format" << std::endl;
		return false;
	}

	Bitmap bmp(FreeImage_GetWidth(image), FreeImage_GetHeight(image));
	
	BYTE *pixels = FreeImage_GetBits(image);

	// BGRA to RGBA
	for (UINT i = 0; i < bmp.getWidth() * bmp.getHeight(); i++)
	{
		bmp.setPixel(i, pixels[i * 4 + 2],
						pixels[i * 4 + 1],
						pixels[i * 4 + 0],
						pixels[i * 4 + 3]);
	}

	FreeImage_Unload(image);

	*bitmap = std::move(bmp);
	return true;
}
