#pragma once

#include <string>
#include <iostream>

#include <GL/glew.h>

#include <GL/FreeImage.h>

class Texture
{
public:
	Texture(GLenum textureTarget_, const std::string& fileName_);

	bool load();

	void bind(GLenum textureUnit_);

private:
	std::string fileName;
	GLenum textureTarget;
	GLuint textureObj;
	FIBITMAP* dib;
	BYTE* dataPointer;
};