#pragma once

//#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>
#include <GL/freeglut.h>

#include "mathematics.h"
#include <iostream>

const double STEP_SCALE = 0.1;
const int MARGIN = 70;
const double ANGLE_STEP = 0.01f;

class Camera
{
	int width, height;
	bool onUp, onDown, onLeft, onRight;
	float angleH, angleV;
	int mouseX, mouseY;
	mat4 m_worldPos, m_rotate;

	void init();
	void computeMatrices();
	void update();
public:
	Camera(vec3 pos_, vec3 target_, vec3 up_, int width_, int height_);
	void move(int key);
	void rotate(int x, int y);
	void rotateCamera();
	mat4 getTrans();
	void setSize(int width_, int height_);

	vec3 pos, target, up;
};