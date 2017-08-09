#include "projection.h"

Projection::Projection(float FOV, float width, float height, float zNear, float zFar)
{
	init(FOV, width, height, zNear, zFar);
}

void Projection::init(float FOV, float width, float height, float zNear, float zFar)
{
	const float ar = width / height;
	const float zRange = zNear - zFar;
	const float tanHalfFOV = tanf(FOV / 2.0f);

	trans = mat4(0.0f);

	trans[0][0] = 1.0f / (tanHalfFOV * ar);
	trans[1][1] = 1.0f / tanHalfFOV;
	trans[2][2] = (-zNear - zFar) / zRange;
	trans[2][3] = 2.0f * zFar*zNear / zRange;
	trans[3][2] = 1.0f;
}

mat4 Projection::getTrans()
{
	return trans;
}
