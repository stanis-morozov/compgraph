#pragma once

#include "mathematics.h"

class Vertex
{
public:
	Vertex(vec3 pos_, vec2 text_);
	Vertex(vec3 pos_, vec2 text_, vec3 norm_);
	Vertex(vec3 pos_, vec2 text_, vec3 norm_, vec3 tang_);

	vec3 position;
	vec2 texture;
	vec3 normal;
	vec3 tangent;
};

void calculateNormals(unsigned *ind, size_t ind_size, Vertex *v, size_t v_sz);