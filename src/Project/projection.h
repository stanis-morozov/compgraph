#pragma once

//#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>

#include "mathematics.h"

class Projection
{
public:
	Projection(float FOV, float width, float height, float zNear, float zFar);

	void init(float FOV, float width, float height, float zNear, float zFar);

	mat4 getTrans();
private:
	mat4 trans;
};