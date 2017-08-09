#include "mathematics.h"

vec2::vec2()
{
	x = y = 0;
}

vec2::vec2(float x_)
{
	x = y = x_;
}

vec2::vec2(float x_, float y_)
{
	x = x_;
	y = y_;
}

vec2 vec2::operator+(const vec2 &v) const
{
	vec2 res;
	res.x = x + v.x;
	res.y = y + v.y;
	return res;
}

vec2 vec2::operator-(const vec2 &v) const
{
	vec2 res;
	res.x = x - v.x;
	res.y = y - v.y;
	return res;
}

vec2 vec2::operator*(float a) const
{
	vec2 res = *this;
	res.x *= a;
	res.y *= a;
	return res;
}

vec2 vec2::operator/(float a) const
{
	vec2 res = *this;
	res.x /= a;
	res.y /= a;
	return res;
}

float vec2::length() const
{
	return sqrt(x * x + y * y);
}

vec2 vec2::normalized() const
{
	return (*this) / length();
}

vec2 operator*(float a, const vec2 &v)
{
	return v * a;
}

vec2 operator/(float a, const vec2 &v)
{
	return v / a;
}

float dot(const vec2& u, const vec2& v)
{
	return u.x * v.x + u.y * v.y;
}



vec3::vec3()
{
	x = y = z = 0;
}

vec3::vec3(float x_)
{
	x = y = z = x_;
}

vec3::vec3(float x_, float y_, float z_)
{
	x = x_;
	y = y_;
	z = z_;
}

vec3 vec3::operator+(const vec3 &v) const
{
	vec3 res;
	res.x = x + v.x;
	res.y = y + v.y;
	res.z = z + v.z;
	return res;
}

vec3 vec3::operator-(const vec3 &v) const
{
	vec3 res;
	res.x = x - v.x;
	res.y = y - v.y;
	res.z = z - v.z;
	return res;
}

vec3 vec3::operator*(float a) const
{
	vec3 res = *this;
	res.x *= a;
	res.y *= a;
	res.z *= a;
	return res;
}

vec3 vec3::operator/(float a) const
{
	vec3 res = *this;
	res.x /= a;
	res.y /= a;
	res.z /= a;
	return res;
}

float vec3::length() const
{
	return sqrt(x * x + y * y + z * z);
}

vec3 vec3::normalized() const
{
	return (*this) / length();
}

vec3 vec3::rotate(float angle, vec3 axis)
{
	const float sinHalfAngle = sin(angle / 2);
	const float cosHalfAngle = cos(angle / 2);

	const float rX = axis.x * sinHalfAngle;
	const float rY = axis.y * sinHalfAngle;
	const float rZ = axis.z * sinHalfAngle;
	const float rW = cosHalfAngle;
	Quaternion rotationQ(rX, rY, rZ, rW);

	Quaternion conjugateQ = rotationQ.conjugated();
	Quaternion w = rotationQ * (*this) * conjugateQ;

	return vec3(w.x, w.y, w.z);
}

vec3 operator*(float a, const vec3 &v)
{
	return v * a;
}

vec3 operator/(float a, const vec3 &v)
{
	return v / a;
}

float dot(const vec3& u, const vec3& v)
{
	return u.x * v.x + u.y * v.y + u.z * v.z;
}

vec3 cross(const vec3&u, const vec3&v)
{
	return vec3(u.y * v.z - u.z * v.y, u.z * v.x - u.x * v.z, u.x * v.y - u.y * v.x);
}



vec4::vec4()
{
	x = y = z = w = 0;
}

vec4::vec4(float x_)
{
	x = y = z = w = x_;
}

vec4::vec4(float x_, float y_, float z_, float w_)
{
	x = x_;
	y = y_;
	z = z_;
	w = w_;
}

vec4 vec4::operator+(const vec4 &v) const
{
	vec4 res;
	res.x = x + v.x;
	res.y = y + v.y;
	res.z = z + v.z;
	res.w = w + v.w;
	return res;
}

vec4 vec4::operator-(const vec4 &v) const
{
	vec4 res;
	res.x = x - v.x;
	res.y = y - v.y;
	res.z = z - v.z;
	res.w = w - v.w;
	return res;
}

vec4 vec4::operator*(float a) const
{
	vec4 res = *this;
	res.x *= a;
	res.y *= a;
	res.z *= a;
	res.w *= a;
	return res;
}

vec4 vec4::operator/(float a) const
{
	vec4 res = *this;
	res.x /= a;
	res.y /= a;
	res.z /= a;
	res.w /= a;
	return res;
}

float vec4::length() const
{
	return sqrt(x * x + y * y + z * z + w * w);
}

vec4 vec4::normalized() const
{
	return (*this) / length();
}

vec4 operator*(float a, const vec4 &v)
{
	return v * a;
}

vec4 operator/(float a, const vec4 &v)
{
	return v / a;
}

float dot(const vec4& u, const vec4& v)
{
	return u.x * v.x + u.y * v.y + u.z * v.z + u.w * v.w;
}



mat4::mat4()
{
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			m[i][j] = 0;
		}
	}
}

mat4::mat4(float x_)
{
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			m[i][j] = 0;
		}
	}
	m[0][0] = m[1][1] = m[2][2] = m[3][3] = x_;
}

mat4::mat4(float x_, float y_, float z_, float w_)
{
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			m[i][j] = 0;
		}
	}
	m[0][0] = x_;
	m[1][1] = y_;
	m[2][2] = z_;
	m[3][3] = w_;
}

mat4::mat4(vec4 v)
{
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			m[i][j] = 0;
		}
	}
	m[0][0] = v.x;
	m[1][1] = v.y;
	m[2][2] = v.z;
	m[3][3] = v.w;
}

mat4 mat4::operator+(const mat4 &v) const
{
	mat4 res = *this;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			res.m[i][j] += v.m[i][j];
		}
	}
	return res;
}

mat4 mat4::operator-(const mat4 &v) const
{
	mat4 res = *this;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			res.m[i][j] -= v.m[i][j];
		}
	}
	return res;
}

mat4 mat4::operator*(const mat4 &v) const
{
	mat4 res;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			for (int k = 0; k < 4; k++) {
				res.m[i][j] += m[i][k] * v.m[k][j];
			}
		}
	}
	return res;
}

vec4 mat4::operator*(const vec4 &v) const
{
	vec4 res;
	res.x = m[0][0] * v.x + m[0][1] * v.y + m[0][2] * v.z + m[0][3] * v.w;
	res.y = m[1][0] * v.x + m[1][1] * v.y + m[1][2] * v.z + m[1][3] * v.w;
	res.z = m[2][0] * v.x + m[2][1] * v.y + m[2][2] * v.z + m[2][3] * v.w;
	res.w = m[3][0] * v.x + m[3][1] * v.y + m[3][2] * v.z + m[3][3] * v.w;
	return res;
}

mat4 mat4::operator*(float a) const
{
	mat4 res = *this;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			res.m[i][j] *= a;
		}
	}
	return res;
}

mat4 mat4::operator/(float a) const
{
	mat4 res = *this;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			res.m[i][j] /= a;
		}
	}
	return res;
}

float * mat4::operator[](int i)
{
	return (float*)m[i];
}

mat4 operator*(float a, const mat4 &v)
{
	return v * a;
}

mat4 operator/(float a, const mat4 &v)
{
	return v / a;
}

Quaternion::Quaternion()
{
	x = y = z = w = 0;
}

Quaternion::Quaternion(float x_, float y_, float z_, float w_)
{
	x = x_;
	y = y_;
	z = z_;
	w = w_;
}

Quaternion::Quaternion(vec4 v)
{
	x = v.x;
	y = v.y;
	z = v.z;
	w = v.w;
}

Quaternion Quaternion::normalized() const
{
	Quaternion q = *this;
	float l = sqrt(x * x + y * y + z * z + w * w);
	q.x /= l;
	q.y /= l;
	q.z /= l;
	q.w /= l;
	return q;
}

Quaternion Quaternion::conjugated() const
{
	Quaternion q = *this;
	q.x = -q.x;
	q.y = -q.y;
	q.z = -q.z;
	return q;
}

Quaternion Quaternion::operator*(const Quaternion &r)
{
	float w_ = (w * r.w) - (x * r.x) - (y * r.y) - (z * r.z);
	float x_ = (x * r.w) + (w * r.x) + (y * r.z) - (z * r.y);
	float y_ = (y * r.w) + (w * r.y) + (z * r.x) - (x * r.z);
	float z_ = (z * r.w) + (w * r.z) + (x * r.y) - (y * r.x);

	return Quaternion(x_, y_, z_, w_);
}

Quaternion Quaternion::operator*(const vec3 &v)
{
	float w_ = -(x * v.x) - (y * v.y) - (z * v.z);
	float x_ = (w * v.x) + (y * v.z) - (z * v.y);
	float y_ = (w * v.y) + (z * v.x) - (x * v.z);
	float z_ = (w * v.z) + (x * v.y) - (y * v.x);

	return Quaternion(x_, y_, z_, w_);
}
