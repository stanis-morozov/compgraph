#pragma once

//#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>
#include "mathematics.h"

class Transform
{
public:
	Transform();

	void scale(float scaleX, float scaleY, float scaleZ);

	void position(float x, float y, float z);

	void rotate(float rotateX, float rotateY, float rotateZ);

	mat4 getTrans();
private:
	mat4 m_scale;
	mat4 m_worldPos;
	mat4 m_rotate;
};