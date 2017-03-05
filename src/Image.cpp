#include "Image.h"
#include <iostream>

ImageInfo::ImageInfo(const char * filename):width(0),height(0),pixels(NULL),dib(0)
{
	FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
	//pointer to the image, once loaded
	FIBITMAP *dib(0);
	//check the file signature and deduce its format
	fif = FreeImage_GetFileType(filename, 0);
	//if still unknown, try to guess the file format from the file extension
	if (fif == FIF_UNKNOWN)
		fif = FreeImage_GetFIFFromFilename(filename);
	//if still unkown, return failure
	if (fif == FIF_UNKNOWN)
		return;

	//check that the plugin has reading capabilities and load the file
	if (FreeImage_FIFSupportsReading(fif))																											
		dib = FreeImage_Load(fif, filename);
	//if the image failed to load, return failure
	if (!dib)
		return;

	dib = FreeImage_ConvertTo24Bits(dib);
	pixels = FreeImage_GetBits(dib);
	width = FreeImage_GetWidth(dib);
	height = FreeImage_GetHeight(dib);
}

bool ImageInfo::isOk()
{
	if ((pixels == 0) || (width == 0) || (height == 0))
		return false;
	return true;
}

void ImageInfo::free()
{
	FreeImage_Unload(dib);
}

ImageInfo::~ImageInfo()
{
	free();
}
