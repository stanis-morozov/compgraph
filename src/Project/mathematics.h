#pragma once

#include <cmath>

const double PI = 3.14159265358979323846264;

class vec2
{
public:
	vec2();
	vec2(float x_);
	vec2(float x_, float y_);
	vec2 operator+(const vec2&) const;
	vec2 operator-(const vec2&) const;
	vec2 operator*(float) const;
	vec2 operator/(float) const;
	float length() const;
	vec2 normalized() const;

	friend float dot(const vec2&, const vec2&);

	float x, y;
};

class vec3
{
public:
	vec3();
	vec3(float x_);
	vec3(float x_, float y_, float z_);
	vec3 operator+(const vec3&) const;
	vec3 operator-(const vec3&) const;
	vec3 operator*(float) const;
	vec3 operator/(float) const;
	float length() const;
	vec3 normalized() const;
	vec3 rotate(float angle, vec3 axis);

	friend float dot(const vec3&, const vec3&);
	friend vec3 cross(const vec3&, const vec3&);

	float x, y, z;
};

vec3 operator*(float a, const vec3&);
vec3 operator/(float a, const vec3&);


class vec4
{
public:
	vec4();
	vec4(float x_);
	vec4(float x_, float y_, float z_, float w_);
	vec4 operator+(const vec4&) const;
	vec4 operator-(const vec4&) const;
	vec4 operator*(float) const;
	vec4 operator/(float) const;
	float length() const;
	vec4 normalized() const;

	friend float dot(const vec4&, const vec4&);

	float x, y, z, w;
};

vec4 operator*(float a, const vec4&);
vec4 operator/(float a, const vec4&);

class mat4
{
	float m[4][4];
public:
	mat4();
	mat4(float);
	mat4(float, float, float, float);
	mat4(vec4);
	mat4 operator+(const mat4&) const;
	mat4 operator-(const mat4&) const;
	mat4 operator*(const mat4&) const;
	vec4 operator*(const vec4&) const;
	mat4 operator*(float) const;
	mat4 operator/(float) const;
	float* operator[](int);
};

mat4 operator*(float a, const mat4&);
mat4 operator/(float a, const mat4&);


class Quaternion
{
public:
	Quaternion();
	Quaternion(float x_, float y_, float z_, float w_);
	Quaternion(vec4 v);
	Quaternion normalized() const;
	Quaternion conjugated() const;

	Quaternion operator*(const Quaternion &);
	Quaternion operator*(const vec3 &);

	float x, y, z, w;
};