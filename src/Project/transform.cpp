#include "transform.h"

Transform::Transform()
{
	scale(1.0f, 1.0f, 1.0f);
	position(0.0f, 0.0f, 0.0f);
	rotate(0.0f, 0.0f, 0.0f);
}

void Transform::scale(float scaleX = 1.0f, float scaleY = 1.0f, float scaleZ = 1.0f)
{
	m_scale = mat4(0.0f);
	
	m_scale[0][0] = scaleX;
	m_scale[1][1] = scaleY;
	m_scale[2][2] = scaleZ;
	m_scale[3][3] = 1.0;
}

void Transform::position(float x, float y, float z)
{
	m_worldPos = mat4(1.0f);

	m_worldPos[0][3] = x;
	m_worldPos[1][3] = y;
	m_worldPos[2][3] = z;
}

void Transform::rotate(float rotateX = 0.0f, float rotateY = 0.0f, float rotateZ = 0.0f)
{
	mat4 rotx = mat4(0.0f);
	mat4 roty = mat4(0.0f);
	mat4 rotz = mat4(0.0f);

	rotx[0][0] = 1.0f;
	rotx[3][3] = 1.0f;
	rotx[1][1] = cos(rotateX);
	rotx[1][2] = -sin(rotateX);
	rotx[2][1] = sin(rotateX);
	rotx[2][2] = cos(rotateX);

	roty[1][1] = 1.0f;
	roty[3][3] = 1.0f;
	roty[0][0] = cos(rotateY);
	roty[0][2] = -sin(rotateY);
	roty[2][0] = sin(rotateY);
	roty[2][2] = cos(rotateY);

	rotz[2][2] = 1.0f;
	rotz[3][3] = 1.0f;
	rotz[0][0] = cos(rotateZ);
	rotz[0][1] = -sin(rotateZ);
	rotz[1][0] = sin(rotateZ);
	rotz[1][1] = cos(rotateZ);

	m_rotate = rotz * roty * rotx;
}

mat4 Transform::getTrans()
{
	return m_worldPos * m_rotate * m_scale;
}
