#include "stdafx.h"
#include "FT.h"


FT::StaticInit FT::__static_init;

FT_Library FT::Library;

FT::StaticInit::StaticInit()
{
	FT_Error error = FT_Init_FreeType(&Library);
	
	if (error)
	{
		Library = nullptr;
		error_stream << "Failed to create library" << std::endl;
	}
}

FT::StaticInit::~StaticInit()
{
	FT_Done_FreeType(Library);
}