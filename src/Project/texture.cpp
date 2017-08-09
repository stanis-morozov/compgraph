#define _CRT_SECURE_NO_WARNINGS

#include "texture.h"

Texture::Texture(GLenum textureTarget_, const std::string& fileName_)
{
	textureTarget = textureTarget_;
	fileName = fileName_;
	dib = NULL;
}

bool Texture::load()
{
	FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;

	fif = FreeImage_GetFileType(fileName.c_str(), 0);
	if (fif == FIF_UNKNOWN)
		fif = FreeImage_GetFIFFromFilename(fileName.c_str());

	if (fif == FIF_UNKNOWN)
		return 0;

	if (FreeImage_FIFSupportsReading(fif))
		dib = FreeImage_Load(fif, fileName.c_str());
	if (!dib)
		return 0;

	dataPointer = FreeImage_GetBits(dib);
	int width = FreeImage_GetWidth(dib);
	int height = FreeImage_GetHeight(dib);
	int iBPP = FreeImage_GetBPP(dib);

	if (dataPointer == NULL || width == 0 || height == 0)
		return false;

	glGenTextures(1, &textureObj);
	glBindTexture(textureTarget, textureObj);

	int iFormat = iBPP == 24 ? GL_BGR : iBPP == 8 ? GL_LUMINANCE : 0;
	int iInternalFormat = iBPP == 24 ? GL_RGB : GL_DEPTH_COMPONENT;

	glTexImage2D(textureTarget, 0, GL_RGB, width, height, 0, iFormat, GL_UNSIGNED_BYTE, dataPointer);

	glTexParameterf(textureTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(textureTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	FreeImage_Unload(dib);
	return 1;
}

void Texture::bind(GLenum textureUnit_)
{
	glActiveTexture(textureUnit_);
	glBindTexture(textureTarget, textureObj);
}
