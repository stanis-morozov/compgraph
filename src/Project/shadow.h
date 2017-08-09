#pragma once

#include <GL/glew.h>

#include <cstdlib>
#include <cstdio>

class ShadowMapFBO
{
public:
	ShadowMapFBO();

	~ShadowMapFBO();

	bool init(unsigned int windowWidth, unsigned int windowHeight);

	void bindForWriting();

	void bindForReading(GLenum textureUnit);

private:
	GLuint fbo;
	GLuint shadowMap;
};