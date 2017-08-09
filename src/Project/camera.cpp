#include "camera.h"

void Camera::init()
{
	vec3 hTarget(target.x, 0.0, target.z);
	hTarget = hTarget.normalized();

	if (hTarget.z >= 0.0f)
	{
		if (hTarget.x >= 0.0f)
		{
			angleH = 2 * PI - asin(hTarget.z);
		}
		else
		{
			angleH = PI + asin(hTarget.z);
		}
	}
	else
	{
		if (hTarget.x >= 0.0f)
		{
			angleH = asin(-hTarget.z);
		}
		else
		{
			angleH = PI / 2 + asin(-hTarget.z);
		}
	}

	angleV = -asin(target.y);

	onUp = false;
	onDown = false;
	onLeft = false;
	onRight = false;
	mouseX = width / 2;
	mouseY = height / 2;
}

void Camera::computeMatrices()
{
	m_worldPos = mat4(1.0f);

	m_worldPos[0][3] = -pos.x;
	m_worldPos[1][3] = -pos.y;
	m_worldPos[2][3] = -pos.z;

	vec3 target_, up_;
	target_ = target.normalized();
	up_ = up.normalized();

	up_ = cross(up_, target_);
	vec3 v = cross(target_, up_);

	m_rotate = mat4(0.0f);
	m_rotate[3][3] = 1.0f;
	m_rotate[0][0] = up_.x;
	m_rotate[0][1] = up_.y;
	m_rotate[0][2] = up_.z;

	m_rotate[1][0] = v.x;
	m_rotate[1][1] = v.y;
	m_rotate[1][2] = v.z;

	m_rotate[2][0] = target_.x;
	m_rotate[2][1] = target_.y;
	m_rotate[2][2] = target_.z;
}

void Camera::update()
{
	vec3 vAxis(0.0f, 1.0f, 0.0f);

	vec3 view(1.0f, 0.0f, 0.0f);
	view = view.rotate(angleH, vAxis);
	view = view.normalized();

	vec3 hAxis = cross(vAxis, view);
	hAxis = hAxis.normalized();
	view = view.rotate(angleV, hAxis);

	target = view;
	target = target.normalized();

	up = cross(target, hAxis);
	up = up.normalized();
}

Camera::Camera(vec3 pos_, vec3 target_, vec3 up_, int width_, int height_)
{
	pos = pos_;
	target = target_;
	up = up_;
	width = width_;
	height = height_;

	init();
}

void Camera::move(int key)
{
	vec3 tmp = cross(target, up).normalized();
	switch (key) {
	case GLUT_KEY_UP:
		pos = pos + target * STEP_SCALE;
		break;
	case GLUT_KEY_DOWN:
		pos = pos - target * STEP_SCALE;
		break;
	case GLUT_KEY_LEFT:
		pos = pos + tmp * STEP_SCALE;
		break;
	case GLUT_KEY_RIGHT:
		pos = pos - tmp * STEP_SCALE;
		break;
	}
}

void Camera::rotate(int x, int y)
{
	const int deltaX = x - mouseX;
	const int deltaY = y - mouseY;

	mouseX = x;
	mouseY = y;

	angleH += (float)deltaX / 20.0f / 180.0f * PI;
	angleV += (float)deltaY / 20.0f / 180.0f * PI;

	if (deltaX == 0) {
		if (x <= MARGIN) {
			onLeft = true;
		}
		else if (x >= (width - MARGIN)) {
			onRight = true;
		}
	}
	else {
		onLeft = false;
		onRight = false;
	}

	if (deltaY == 0) {
		if (y <= MARGIN) {
			onUp = true;
		}
		else if (y >= (height - MARGIN)) {
			onDown = true;
		}
	}
	else {
		onUp = false;
		onDown = false;
	}

	update();
}

void Camera::rotateCamera()
{
	int shouldUpdate = 0;

	if (onLeft) {
		angleH -= ANGLE_STEP;
		shouldUpdate = 1;
	}
	else if (onRight) {
		angleH += ANGLE_STEP;
		shouldUpdate = 1;
	}

	if (onUp) {
		if (angleV > -PI / 2) {
			angleV -= ANGLE_STEP;
			shouldUpdate = 1;
		}
	}
	else if (onDown) {
		if (angleV < PI / 2) {
			angleV += ANGLE_STEP;
			shouldUpdate = 1;
		}
	}

	if (shouldUpdate) {
		update();
	}
}


mat4 Camera::getTrans()
{
	computeMatrices();

	return m_rotate * m_worldPos;
}

void Camera::setSize(int width_, int height_)
{
	width = width_;
	height = height_;
}
